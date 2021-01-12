#include <App.hpp>
#include <ConfigAttributes.hpp>
#include "WebPages.h"

void paramChars(char *dest, const char *paramName, const char *name, const char *value)
{
  if (strcmp(name, paramName) == 0)
  {
    strncpy(dest, value, 63);
    dest[63] = 0;
  }
}

void paramInt(int *dest, const char *paramName, const char *name, const char *value)
{
  if (strcmp(name, paramName) == 0)
  {
    int v = 0;

    if (value != 0 && strlen(value) > 0)
    {
      v = atoi(value);
    }

    *dest = v;
  }
}

void paramUnsignedLong(unsigned long *dest, const char *paramName, const char *name,
                       const char *value)
{
  if (strcmp(name, paramName) == 0)
  {
    unsigned long v = 0;

    if (value != 0 && strlen(value) > 0)
    {
      v = atol(value);
    }

    *dest = v;
  }
}

void paramBool(bool *dest, const char *paramName, const char *name, const char *value)
{
  if (strcmp(name, paramName) == 0)
  {
    if (value != 0 && strlen(value) > 0)
    {
      *dest = strcmp("true", value) == 0;
    }
  }
}

void storeConfigValue(const char *name, const char *value)
{
  // Security
  paramChars(appcfgWR.admin_password, A_admin_password, name, value);

  // OTA
  paramBool(&appcfgWR.ota_enabled, A_ota_enabled, name, value);
  paramChars(appcfgWR.ota_hostname, A_ota_hostname, name, value);
  paramChars(appcfgWR.ota_password, A_ota_password, name, value);

  // WIFI
  paramInt(&appcfgWR.wifi_mode, A_wifi_mode, name, value);
  paramChars(appcfgWR.wifi_ssid, A_wifi_ssid, name, value);
  paramChars(appcfgWR.wifi_password, A_wifi_password, name, value);

  // Network
  paramInt(&appcfgWR.net_mode, A_net_mode, name, value);
  paramChars(appcfgWR.net_host, A_net_host, name, value);
  paramChars(appcfgWR.net_gateway, A_net_gateway, name, value);
  paramChars(appcfgWR.net_mask, A_net_mask, name, value);
  paramChars(appcfgWR.net_dns, A_net_dns, name, value);

  // MQTT
  paramBool(&appcfgWR.mqtt_enabled, A_mqtt_enabled, name, value);
  paramChars(appcfgWR.mqtt_clientid, A_mqtt_clientid, name, value);
  paramChars(appcfgWR.mqtt_host, A_mqtt_host, name, value);
  paramInt(&appcfgWR.mqtt_port, A_mqtt_port, name, value);
  paramBool(&appcfgWR.mqtt_useauth, A_mqtt_useauth, name, value);
  paramChars(appcfgWR.mqtt_user, A_mqtt_user, name, value);
  paramChars(appcfgWR.mqtt_password, A_mqtt_password, name, value);
  paramChars(appcfgWR.mqtt_intopic, A_mqtt_intopic, name, value);
  paramChars(appcfgWR.mqtt_outtopic, A_mqtt_outtopic, name, value);

  paramUnsignedLong(&appcfgWR.mqtt_sending_interval, A_mqtt_sending_interval, name,
                    value);

  // Telnet
  paramBool(&appcfgWR.telnet_enabled, A_telnet_enabled, name, value);

  // NTP
  paramBool(&appcfgWR.ntp_enabled, A_ntp_enabled, name, value);
  paramChars(appcfgWR.ntp_timezone, A_ntp_timezone, name, value);
  paramChars(appcfgWR.ntp_server1, A_ntp_server1, name, value);
  paramChars(appcfgWR.ntp_server2, A_ntp_server2, name, value);
  paramChars(appcfgWR.ntp_server3, A_ntp_server3, name, value);
}

void handleSaveConfigPage()
{
  sendAuthentication();
  sendHeader(APP_NAME " - Save Config", true);
  sendPrint("<form class='pure-form'>");
  sendLegend("Configuration saved.");

  int numberOfArguments = server.args();
  sendPrint("<pre>");

  memset(&appcfgWR, 0, sizeof(appcfgWR));

  for (int i = 0; i < numberOfArguments - 1; i++)
  {
    const char *argName = server.argName(i).c_str();
    const char *argValue = server.arg(i).c_str();
    storeConfigValue(argName, argValue);
    sendPrintf("%2d. %s = %s\n", (i + 1), argName, argValue);
  }

  sendPrint("</pre><h3 style='color: red'>Restarting System ... takes about 30s</h3></form>");
  sendFooter();
  app.delayedSystemRestart();
}
