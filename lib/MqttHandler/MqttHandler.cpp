#include <App.hpp>
#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFiClient.h>
#include <WifiHandler.hpp>
#include "MqttHandler.hpp"

MqttHandler mqttHandler;

static WiFiClient wifiClient;
static PubSubClient client(wifiClient);
static long lastReconnectAttempt = 0;

static void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  for (unsigned int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }

  Serial.println();
}

MqttHandler::MqttHandler() { initialized = false; }

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

void MqttHandler::handle( unsigned long now)
{
  if (appcfg.mqtt_enabled && wifiHandler.isReady())
  {
    if (initialized == false)
    {
      setup();
    }

    if (!client.connected())
    {
      long now = millis();

      if (now - lastReconnectAttempt > 5000)
      {
        lastReconnectAttempt = now;
        reconnect();
      }
    }
    else
    {
      client.loop();
    }
  }
}

void MqttHandler::sendValue( const char* topic, const char *value )
{
  if( appcfg.mqtt_enabled && wifiHandler.isReady() && client.connected())
  {
    if ( topic != NULL && value != NULL && strlen(topic) > 0 && topic[0] != '-' )
    {
      client.publish( topic, value );
    }
  }
}

void MqttHandler::sendValue( const char* topic, const float value )
{
  char buffer[32];
  sprintf( buffer, "%0.2f", value );
  sendValue( topic, buffer );
}
