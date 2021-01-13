#include <Arduino.h>
#include <App.hpp>
#include <MqttHandler.hpp>
#include <OtaHandler.hpp>
#include <WebHandler.hpp>
#include <WifiHandler.hpp>
#include <uzlib.h>
#include <Wire.h>
#include <TelnetStream.h>
#include <TuringPiHandler.hpp>

unsigned long lifeTicker;
unsigned long maxLoopTime;
unsigned long lastLoopTimestamp;
unsigned long thisLoopTimestamp;

void setup()
{
  app.setup();
  app.writeConfig();
  app.printConfig(appcfg);

  Serial.println("\n");
  LOG1("I2C SCL=%d\n", SCL);
  LOG1("    SDA=%d\n", SDA);

  byte count = 0;

  Wire.begin();

  for (byte i = 8; i < 120; i++)
  {
    Wire.beginTransmission(i);       // Begin I2C transmission Address (i)
    if (Wire.endTransmission() == 0) // Receive 0 = success (ACK response)
    {
      LOG1("Found address: %d (0x%02X)\n", i, i);
      count++;
    }
  }
  LOG1("Found %d device(s).\n\n", count);

  wifiHandler.setup();
  turingPiHandler.setup();
  maxLoopTime = 0l;
  lifeTicker = lastLoopTimestamp = millis();
  uzlib_init();
}

void loop()
{
  thisLoopTimestamp = millis();
  maxLoopTime = max(maxLoopTime, thisLoopTimestamp - lastLoopTimestamp);
  lastLoopTimestamp = thisLoopTimestamp;

  if ((thisLoopTimestamp - lifeTicker) >= 10000)
  {
    LOG1("max loop time = %ld\n", maxLoopTime);
    LOG1("wifi is connected %d\n", wifiHandler.isConnected());
    LOG1("free heap %d\n", ESP.getFreeHeap());
    maxLoopTime = 0l;
    lifeTicker = thisLoopTimestamp;
  }

  if (wifiHandler.handle(thisLoopTimestamp))
  {
    otaHandler.handle();
    webHandler.handle();
    turingPiHandler.handle();
    mqttHandler.handle(thisLoopTimestamp);
    if (appcfg.telnet_enabled)
    {
      TelnetStream.handle();
    }
  }

  app.handle(thisLoopTimestamp);

  delay(20); // time for IP stack
}
