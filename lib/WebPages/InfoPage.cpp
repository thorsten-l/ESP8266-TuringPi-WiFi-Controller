#include "WebPages.h"

void handleInfoPage()
{
  sendHeader(APP_NAME " - Info");
  sendPrint("<form class='pure-form'>");
  sendLegend("Application");
  sendPrint(
      "<p>Name: " APP_NAME "</p>"
      "<p>Version: " APP_VERSION "</p>"
      "<p>PlatformIO Environment: " PIOENV "</p>"
      "<p>Author: Dr. Thorsten Ludewig &lt;t.ludewig@gmail.com></p>");

  sendLegend("Build");
  sendPrint("<p>Date: " __DATE__ "</p>"
            "<p>Time: " __TIME__ "</p>");

  sendLegend("RESTful API");

  sendPrintf(
      "<p><a href='http://%s/espinfo'>http://%s/espinfo</a> - ESP8266 Info</p>",
      WiFi.localIP().toString().c_str(), WiFi.localIP().toString().c_str());
 
  sendPrintf(
      "<p><a href='http://%s/state'>http://%s/state</a> - Slot info</p>",
      WiFi.localIP().toString().c_str(), WiFi.localIP().toString().c_str());
 
  sendPrintf(
      "<p>http://%s/set?slot=[1..7]&amp;power_on=[1|0|true|false] - Set slot power ON|OFF (admin authentication required)</p>",
      WiFi.localIP().toString().c_str(), WiFi.localIP().toString().c_str());
 
  sendLegend("Services");

  sendPrintf("<p>OTA Enabled: %s</p>",
             (appcfg.ota_enabled) ? "true" : "false");
  sendPrintf("<p>MQTT Enabled: %s</p>",
             (appcfg.mqtt_enabled) ? "true" : "false");
  sendPrintf("<p>Telnet Enabled: %s</p>",
             (appcfg.telnet_enabled) ? "true" : "false");
  sendPrintf("<p>NTP (Timeservice) Enabled: %s</p>",
             (appcfg.ntp_enabled) ? "true" : "false");

  sendPrint("</form>");
  sendFooter();
}
