#include "WebPages.h"

#include <ConfigAttributes.hpp>
#include <LinkedList.hpp>
#include <WifiHandler.hpp>
#include <Util.hpp>
#include <html/setup.h>

const char *selected = "selected";
const char *checked = "checked";
const char *blank = "";

static const char *setupProcessor(const char *var)
{
  if (strcmp(var, A_admin_password) == 0)
    return appcfg.admin_password;

  // Wifi

  if (strcmp(var, "wifi_mode_ap") == 0 && appcfg.wifi_mode == WIFI_AP)
    return selected;
  if (strcmp(var, "wifi_mode_station") == 0 && appcfg.wifi_mode == WIFI_STA)
    return selected;

  if (strcmp(var, "scanned_network_options") == 0)
  {
    ListNode *node = wifiHandler.getScannedNetworks();
    String options = "";

    if (node == NULL)
    {
      LOG0("node==NULL\n");
    }

    while (node != NULL)
    {
      options += F("<option>");
      options += String(*node->value);
      options += F("</option>");
      node = node->next;
    }

    fillBuffer(options.c_str());
    return buffer;
  }

  if (strcmp(var, A_wifi_ssid) == 0)
    return appcfg.wifi_ssid;
  if (strcmp(var, A_wifi_password) == 0)
    return appcfg.wifi_password;

  // Network
  if (strcmp(var, "net_mode_dhcp") == 0 && appcfg.net_mode == NET_MODE_DHCP)
    return selected;
  if (strcmp(var, "net_mode_static") == 0 && appcfg.net_mode == NET_MODE_STATIC)
    return selected;
  if (strcmp(var, A_net_host) == 0)
    return appcfg.net_host;
  if (strcmp(var, A_net_gateway) == 0)
    return appcfg.net_gateway;
  if (strcmp(var, A_net_mask) == 0)
    return appcfg.net_mask;
  if (strcmp(var, A_net_dns) == 0)
    return appcfg.net_dns;

  // OTA
  if (strcmp(var, A_ota_hostname) == 0)
    return appcfg.ota_hostname;
  if (strcmp(var, A_ota_password) == 0)
    return appcfg.ota_password;
  if (strcmp(var, A_ota_enabled) == 0 && appcfg.ota_enabled == true)
    return checked;

  // MQTT
  if (strcmp(var, A_mqtt_enabled) == 0 && appcfg.mqtt_enabled == true)
    return checked;
  if (strcmp(var, A_mqtt_clientid) == 0)
    return appcfg.mqtt_clientid;
  if (strcmp(var, A_mqtt_host) == 0)
    return appcfg.mqtt_host;
  if (strcmp(var, A_mqtt_port) == 0)
  {
    sprintf(buffer, "%d", appcfg.mqtt_port);
    return buffer;
  }

  if (strcmp(var, A_mqtt_useauth) == 0 && appcfg.mqtt_useauth == true)
    return checked;
  if (strcmp(var, A_mqtt_user) == 0)
    return appcfg.mqtt_user;
  if (strcmp(var, A_mqtt_password) == 0)
    return appcfg.mqtt_password;
  if (strcmp(var, A_mqtt_intopic) == 0)
    return appcfg.mqtt_intopic;
  if (strcmp(var, A_mqtt_outtopic) == 0)
    return appcfg.mqtt_outtopic;
  if (strcmp(var, A_mqtt_sending_interval) == 0)
  {
    sprintf(buffer, "%lu", appcfg.mqtt_sending_interval);
    return buffer;
  }

  // Telnet
  if (strcmp(var, A_telnet_enabled) == 0 && appcfg.telnet_enabled == true)
    return checked;

  // NTP
  if (strcmp(var, A_ntp_enabled) == 0 && appcfg.ntp_enabled == true)
    return checked;
  if (strcmp(var, A_ntp_timezone) == 0)
    return appcfg.ntp_timezone;
  if (strcmp(var, A_ntp_server1) == 0)
    return appcfg.ntp_server1;
  if (strcmp(var, A_ntp_server2) == 0)
    return appcfg.ntp_server2;
  if (strcmp(var, A_ntp_server3) == 0)
    return appcfg.ntp_server3;

  return nullptr;
}

void handleSetupPage()
{
  sendAuthentication();
  sendHeader(APP_NAME " - Setup");
  sendHtmlTemplate(SETUP_HTML_TEMPLATE, setupProcessor);
  sendFooter();
}


/*
String setupProcessor(const String &var)
{
  String selected = F("selected");
  String checked = F("checked");

  if (var == A_admin_password)
    return String(appcfg.admin_password);

  // Wifi
  if (var == "wifi_mode_ap" && appcfg.wifi_mode == WIFI_AP)
    return selected;
  if (var == "wifi_mode_station" && appcfg.wifi_mode == WIFI_STA)
    return selected;

  if (var == "scanned_network_options")
  {
    ListNode *node = wifiHandler.getScannedNetworks();
    String options = "";

    while (node != NULL)
    {
      options += F("<option>");
      options += String(*node->value);
      options += F("</option>");
      node = node->next;
    }

    return options;
  }

  if (var == A_wifi_ssid)
    return String(appcfg.wifi_ssid);
  if (var == A_wifi_password)
    return String(appcfg.wifi_password);

  // Network
  if (var == "net_mode_dhcp" && appcfg.net_mode == NET_MODE_DHCP)
    return selected;
  if (var == "net_mode_static" && appcfg.net_mode == NET_MODE_STATIC)
    return selected;
  if (var == A_net_host)
    return String(appcfg.net_host);
  if (var == A_net_gateway)
    return String(appcfg.net_gateway);
  if (var == A_net_mask)
    return String(appcfg.net_mask);
  if (var == A_net_dns)
    return String(appcfg.net_dns);

  // OTA
  if (var == A_ota_hostname)
    return String(appcfg.ota_hostname);
  if (var == A_ota_password)
    return String(appcfg.ota_password);

  // OpenHAB
  if (var == A_ohab_enabled && appcfg.ohab_enabled == true)
    return checked;
  if (var == "ohab_v1" && appcfg.ohab_version == 1)
    return selected;
  if (var == "ohab_v2" && appcfg.ohab_version == 2)
    return selected;
  if (var == A_ohab_itemname)
    return String(appcfg.ohab_itemname);
  if (var == A_ohab_host)
    return String(appcfg.ohab_host);
  if (var == A_ohab_port)
    return String(appcfg.ohab_port);
  if (var == A_ohab_useauth && appcfg.ohab_useauth == true)
    return checked;
  if (var == A_ohab_user)
    return String(appcfg.ohab_user);
  if (var == A_ohab_password)
    return String(appcfg.ohab_password);

#ifdef HAVE_ENERGY_SENSOR
  if (var == A_ohab_item_voltage)
    return String(appcfg.ohab_item_voltage);
  if (var == A_ohab_item_current)
    return String(appcfg.ohab_item_current);
  if (var == A_ohab_item_power)
    return String(appcfg.ohab_item_power);
  if (var == A_ohab_sending_interval)
    return String(appcfg.ohab_sending_interval);
#endif

  // Alexa
  if (var == A_alexa_enabled && appcfg.alexa_enabled == true)
    return checked;
  if (var == A_alexa_devicename)
    return String(appcfg.alexa_devicename);

  // MQTT
  if (var == A_mqtt_enabled && appcfg.mqtt_enabled == true)
    return checked;
  if (var == A_mqtt_clientid)
    return String(appcfg.mqtt_clientid);
  if (var == A_mqtt_host)
    return String(appcfg.mqtt_host);
  if (var == A_mqtt_port)
    return String(appcfg.mqtt_port);
  if (var == A_mqtt_useauth && appcfg.mqtt_useauth == true)
    return checked;
  if (var == A_mqtt_user)
    return String(appcfg.mqtt_user);
  if (var == A_mqtt_password)
    return String(appcfg.mqtt_password);
  if (var == A_mqtt_intopic)
    return String(appcfg.mqtt_intopic);
  if (var == A_mqtt_outtopic)
    return String(appcfg.mqtt_outtopic);
#ifdef HAVE_ENERGY_SENSOR
  if (var == A_mqtt_topic_voltage)
    return String(appcfg.mqtt_topic_voltage);
  if (var == A_mqtt_topic_current)
    return String(appcfg.mqtt_topic_current);
  if (var == A_mqtt_topic_power)
    return String(appcfg.mqtt_topic_power);
  if (var == A_mqtt_topic_json)
    return String(appcfg.mqtt_topic_json);
  if (var == A_mqtt_sending_interval)
    return String(appcfg.mqtt_sending_interval);
#endif

  // Syslog
  if (var == A_syslog_enabled && appcfg.syslog_enabled == true)
    return checked;
  if (var == A_syslog_host)
    return String(appcfg.syslog_host);
  if (var == A_syslog_port)
    return String(appcfg.syslog_port);
  if (var == A_syslog_app_name)
    return String(appcfg.syslog_app_name);

  if (var == "millis")
    return String(millis());

#ifdef POWER_BUTTON_IS_MULTIMODE
  // Power Button Mode
  if (var == "power_button_mode_switch" && appcfg.power_button_mode == POWER_BUTTON_MODE_SWITCH)
    return selected;
  if (var == "power_button_mode_toggle" && appcfg.power_button_mode == POWER_BUTTON_MODE_TOGGLE)
    return selected;
  if (var == "power_button_mode_toggle_switch" && appcfg.power_button_mode == POWER_BUTTON_MODE_TOGGLE_SWITCH)
    return selected;
#endif

#ifdef WIFI_LED
  if (var == A_led_night_mode_enabled && appcfg.led_night_mode_enabled == true)
    return checked;
  if (var == A_led_night_mode_timeout)
    return String(appcfg.led_night_mode_timeout);
#endif

  if (var == A_inet_check_enabled && appcfg.inet_check_enabled == true)
    return checked;
  if (var == A_inet_check_period)
    return String(appcfg.inet_check_period);

  String ica = String( A_inet_check_action );
  ica += "_";
  ica += appcfg.inet_check_action;
  
  if ( var == ica )
    return selected;

  return String();
}

void handleSetupPage(AsyncWebServerRequest *request)
{
  LOG0("handleSetupPage\n");
  if (!request->authenticate("admin", appcfg.admin_password))
  {
    LOG0("authenticate\n");
    return request->requestAuthentication();
  }

  LOG0("build response\n");
  AsyncWebServerResponse *response =
      request->beginResponse_P(200, "text/html", SETUP_HTML, setupProcessor);
  response->addHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  response->addHeader("Pragma", "no-cache");
  response->addHeader("Expires", "0");

  LOG0("send response\n");
  request->send(response);
}

*/
