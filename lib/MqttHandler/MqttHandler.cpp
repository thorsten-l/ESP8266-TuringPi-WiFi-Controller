#include <App.hpp>
#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFiClient.h>
#include <WifiHandler.hpp>
#include <WebPages.h>
#include <Util.hpp>
#include <TuringPiHandler.hpp>
#include "MqttHandler.hpp"

MqttHandler mqttHandler;

static WiFiClient wifiClient;
static PubSubClient client(wifiClient);
static long lastReconnectAttempt = 0;

static void callback(char *topic, byte *payload, unsigned int length)
{
  // set_slot 1 power_on 0
  char command[32];
  char subcommand[32];
  int slot = -1;
  int power_on = -1;

  unsigned int i;
  for (i = 0; i < length; i++)
  {
    buffer2[i] = (char)payload[i];
  }
  buffer2[i] = 0;
  buffer2[31] = 0; // truncate
  logMessage("MQTT: message arrived [%s] = %s", topic, buffer2);
  sscanf( buffer2, "%s %d %s %d", command, &slot, subcommand, &power_on );
  if ( strcmp( "set_slot", command ) == 0 
    && strcmp( "power_on", subcommand ) == 0
    && slot >= 1 && slot <= 7
    && power_on >= 0 && power_on <= 1
  )
  {
    logMessage("MQTT: turning slot %d to power %s.", slot, (power_on==1)?"on":"off");
    turingPiHandler.setPower( slot, power_on == 1 );
  }
  else
  {
    logMessage("MQTT(ERROR): unknown command");
  }
  
}

MqttHandler::MqttHandler()
{
  initialized = false;
  lastPublishTimestamp = 0l;
}

bool MqttHandler::reconnect()
{
  if ((appcfg.mqtt_useauth &&
       client.connect(appcfg.mqtt_clientid, appcfg.mqtt_user,
                      appcfg.mqtt_password)) ||
      (!appcfg.mqtt_useauth && client.connect(appcfg.mqtt_clientid)))
  {
    LOG0("mqtt broker connected\n");
    client.subscribe(appcfg.mqtt_intopic);
  }

  return client.connected();
}

////////

void MqttHandler::setup()
{
  LOG0("MQTT Setup...\n");
  client.setServer(appcfg.mqtt_host, appcfg.mqtt_port);
  client.setCallback(callback);
  initialized = true;
}

void MqttHandler::handle(unsigned long now)
{
  if (appcfg.mqtt_enabled && wifiHandler.isReady())
  {
    if (initialized == false)
    {
      setup();
    }

    if (!client.connected())
    {
      now = millis();

      if (now - lastReconnectAttempt > 5000)
      {
        lastReconnectAttempt = now;
        reconnect();
      }
    }
    else
    {
      client.loop();

      if (appcfg.mqtt_sending_interval > 0 
         && (now - lastPublishTimestamp) >= (appcfg.mqtt_sending_interval * 1000))
      {
        buildJsonMessage();
        sendValue(appcfg.mqtt_outtopic, buffer);
        lastPublishTimestamp = now;
      }
    }
  }
}

void MqttHandler::sendValue(const char *topic, const char *value)
{
  if (appcfg.mqtt_enabled && wifiHandler.isReady() && client.connected())
  {
    if (topic != NULL && value != NULL && strlen(topic) > 0 && topic[0] != '-')
    {
      client.publish(topic, value);
    }
  }
}

void MqttHandler::sendValue(const char *topic, const float value)
{
  char buffer[32];
  sprintf(buffer, "%0.2f", value);
  sendValue(topic, buffer);
}
