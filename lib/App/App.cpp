#include "App.hpp"
#include "DefaultAppConfig.h"
#include <FS.h>
#include <LittleFS.h>
#include <MicroJson.hpp>
#include <Util.hpp>
#include <Development.h>
#include <TelnetStream.h>

struct tm appTimeinfo;
char appUptimeBuffer[64];
char appLocalIpBuffer[32];
char appDateTimeBuffer[32];

App app;
AppConfig appcfg;
AppConfig appcfgWR;
AppConfig appcfgRD;


void logMessage(const char *message)
{
  static char mbuffer[MAX_MESSAGE_LENGTH + 1];
  static bool messageStartRollover = false;

  sprintf(mbuffer, "(%s) %s", appDateTime(), message);
  TelnetStream.println(mbuffer);
  Serial.println(mbuffer);

  // put
  char *mb = messageBuffer + (messageEndIndex * (MAX_MESSAGE_LENGTH + 1));

  strncpy(mb, mbuffer, MAX_MESSAGE_LENGTH);
  mb[MAX_MESSAGE_LENGTH] = 0;

  messageEndIndex += 1;
  messageEndIndex %= MESSAGE_BUFFER_LINES;

  if (messageEndIndex == 0)
  {
    messageStartRollover = true;
  }

  if (messageStartRollover)
  {
    messageStartIndex += 1;
    messageStartIndex %= MESSAGE_BUFFER_LINES;
  }
}

void logMessage(String format, ...)
{
  va_list myargs;
  va_start(myargs, format);
  vsprintf(buffer, format.c_str(), myargs);
  va_end(myargs);
  logMessage(buffer);
}

char *formatChipId( char *attribute )
{
  snprintf( buffer, 63, attribute, ESP.getChipId());
  buffer[63]= 0;
  strncpy(attribute,buffer,63);
  return attribute;
}

const char *appUptime()
{
  time_t uptime = millis() / 1000;
  int uptimeSeconds = uptime % 60;
  int uptimeMinutes = (uptime / 60) % 60;
  int uptimeHours = (uptime / 3600) % 24;
  time_t uptimeDays = (uptime / 86400);
  sprintf(appUptimeBuffer, "%ld days, %d hours, %d minutes, %d seconds", uptimeDays, uptimeHours, uptimeMinutes,
          uptimeSeconds);
  return appUptimeBuffer;
}

const char *appDateTime(time_t now)
{
  localtime_r(&now, &appTimeinfo);
  sprintf(appDateTimeBuffer, "%4d-%02d-%02d %02d:%02d:%02d", appTimeinfo.tm_year + 1900, appTimeinfo.tm_mon + 1,
          appTimeinfo.tm_mday, appTimeinfo.tm_hour, appTimeinfo.tm_min, appTimeinfo.tm_sec);
  return appDateTimeBuffer;
}

const char *appDateTime()
{
  return appDateTime(time(nullptr));
}

void appShowHeader(Stream &out)
{
  out.println("\n\n" APP_NAME " - " APP_VERSION " - " APP_AUTHOR);
  out.println("BUILD: " __DATE__ " " __TIME__);
  out.println("PIOENV: " PIOENV);
  out.println("PIOPLATFORM: " PIOPLATFORM);
  out.println("PIOFRAMEWORK: " PIOFRAMEWORK);
  out.printf("ESP SDK Version: %s\n", ESP.getSdkVersion());
  out.printf("ESP Core Version: %s\n\n", ESP.getCoreVersion().c_str());
}

App::App()
{
  sprintf(initFilename, "/%08X.ini", ESP.getChipId());
  defaultConfig();
  initSPIFFS = false;
  initialized = true;
  doSystemRestart = false;
}

void App::defaultConfig()
{
  strncpy(appcfg.wifi_ssid, DEFAULT_WIFI_SSID, 63);
  strncpy(appcfg.wifi_password, DEFAULT_WIFI_PASSWORD, 63);
  appcfg.wifi_mode = DEFAULT_WIFI_MODE;

  appcfg.net_mode = DEFAULT_NET_MODE;
  strncpy(appcfg.net_host, DEFAULT_NET_HOST, 63);
  strncpy(appcfg.net_mask, DEFAULT_NET_MASK, 63);
  strncpy(appcfg.net_gateway, DEFAULT_NET_GATEWAY, 63);
  strncpy(appcfg.net_dns, DEFAULT_NET_DNS, 63);

  strncpy(appcfg.ota_hostname, DEFAULT_OTA_HOSTNAME, 63);
  strncpy(appcfg.ota_password, DEFAULT_OTA_PASSWORD, 63);

  strncpy(appcfg.admin_password, DEFAULT_ADMIN_PASSWORD, 63);

  appcfg.mqtt_enabled = DEFAULT_MQTT_ENABLED;
  strncpy(appcfg.mqtt_clientid, DEFAULT_MQTT_CLIENTID, 63);
  strncpy(appcfg.mqtt_host, DEFAULT_MQTT_HOST, 63);
  appcfg.mqtt_port = DEFAULT_MQTT_PORT;
  appcfg.mqtt_useauth = DEFAULT_MQTT_USEAUTH;
  strncpy(appcfg.mqtt_user, DEFAULT_MQTT_USER, 63);
  strncpy(appcfg.mqtt_password, DEFAULT_MQTT_PASSWORD, 63);
  strncpy(appcfg.mqtt_intopic, DEFAULT_MQTT_INTOPIC, 63);
  strncpy(appcfg.mqtt_outtopic, DEFAULT_MQTT_OUTTOPIC, 63);
  appcfg.mqtt_sending_interval = DEFAULT_MQTT_SENDING_INTERVAL;

  appcfg.telnet_enabled = DEFAULT_TELNET_ENABLED;

  appcfg.ntp_enabled = DEFAULT_NTP_ENABLED;
  strncpy(appcfg.ntp_timezone, DEFAULT_NTP_TIMEZONE, 63);
  strncpy(appcfg.ntp_server1, DEFAULT_NTP_SERVER1, 63);
  strncpy(appcfg.ntp_server2, DEFAULT_NTP_SERVER2, 63);
  strncpy(appcfg.ntp_server3, DEFAULT_NTP_SERVER3, 63);
  
  appcfg.ping_enabled = DEFAULT_PING_ENABLED;
  appcfg.ping_interval = DEFAULT_PING_INTERVAL;
  for( int i=0; i<7; i++ ) 
  {
    strncpy(appcfg.ping_addr[i], DEFAULT_PING_IP, 31);
    appcfg.ping_addr[i][31] = 0;
  }

  memcpy(&appcfgWR, &appcfg, sizeof(appcfg));
  memcpy(&appcfgRD, &appcfg, sizeof(appcfg));
}

void App::firmwareReset()
{
  if (LittleFS.begin())
  {
    LOG0("Removing init file\n");
    LittleFS.remove(initFilename);
    LittleFS.end();
  }
  delayedSystemRestart();
}

void App::formatSPIFFS()
{
  wifiLedOn();
  ESP.eraseConfig();

  if (LittleFS.begin())
  {
    LOG0("File system format started...\n");
    LittleFS.format();
    LOG0("File system format finished.\n");
    LittleFS.end();
  }
  else
  {
    LOG0("\nERROR: format filesystem.\n");
  }
  wifiLedOff();
}

void App::restartSystem()
{
  // watchdogTicker.detach();
  ESP.eraseConfig();
  LOG0("*** restarting system ***\n");
  delay(2000);
  ESP.restart();
  delay(2000);
  ESP.reset();
}

void App::setup()
{
  Serial.begin(74880);

#ifdef WIFI_LED
  pinMode( WIFI_LED, OUTPUT );
#endif

  for (int i = 0; i < 5; i++)
  {
    wifiLedOn();
    delay(500);
    wifiLedOff();
    delay(500);
    Serial.println();
  }

  Serial.println("\n\n");
  Serial.println("\n\n");
  Serial.println(F(APP_NAME ", Version " APP_VERSION ", by " APP_AUTHOR));
  Serial.println("Build date: " __DATE__ " " __TIME__);
  Serial.printf("appcfg file size: %d bytes\n\n", sizeof(appcfg));

  showChipInfo();

  if (LittleFS.begin())
  {
    if (LittleFS.exists(initFilename))
    {
      LOG1("Init file %s found.\n", initFilename);
      initSPIFFS = false;
    }
    else
    {
      LOG1("WARNING: Init file %s does not exist.\n", initFilename);
      initSPIFFS = true;
    }
    LittleFS.end();
  }
  else
  {
    initSPIFFS = true;
  }

  if (initSPIFFS == true)
  {
    formatSPIFFS();

    if (LittleFS.begin())
    {
      LOG0("writing init file\n");
      File initFile = LittleFS.open(initFilename, "w");
      initFile.write("true");
      initFile.close();
      LittleFS.end();
    }

    restartSystem();
  }

  ESP.eraseConfig();
  memcpy(&appcfgRD, &appcfg, sizeof(appcfg));
  loadConfig();

///////////////////////////////////////////////////////////////
#ifdef OVERRIDE_WIFI_SETTINGS
  appcfg.wifi_mode = OVERRIDE_WIFI_MODE;
  strcpy( appcfg.wifi_ssid, OVERRIDE_WIFI_SSID );
  strcpy( appcfg.wifi_password, OVERRIDE_WIFI_PASSWORD );
  appcfg.ota_enabled = OVERRIDE_OTA_ENABLED;
#endif
///////////////////////////////////////////////////////////////
  
  formatChipId( appcfg.ota_hostname );
  formatChipId( appcfg.mqtt_clientid );
  formatChipId( appcfg.mqtt_intopic );
  formatChipId( appcfg.mqtt_outtopic );

  memcpy(&appcfgWR, &appcfg, sizeof(appcfg));

  configTime(appcfg.ntp_timezone, appcfg.ntp_server1, appcfg.ntp_server2, appcfg.ntp_server3);
}

void App::loadConfig()
{
  if (!LittleFS.begin())
  {
    LOG0("ERROR: Failed to mount file system");
  }
  else
  {
    if (LittleFS.exists(APP_CONFIG_FILE_JSON))
    {
      if (loadJsonConfig(APP_CONFIG_FILE_JSON) == false)
      {
        memcpy(&appcfg, &appcfgRD, sizeof(appcfg));
      }
      else
      {
        LOG0("ERROR: loading config.json file. Using default config.\n");
      }
    }
    else
    {
      LOG0("WARNING: appcfg file " APP_CONFIG_FILE_JSON
           " does not exist. Using default appcfg.\n");
    }
    LittleFS.end();
  }
}

void App::writeConfig()
{
  if (!LittleFS.begin())
  {
    LOG0("ERROR: Failed to mount file system");
  }
  else
  {
    LOG0("Writing " APP_CONFIG_FILE_JSON " file.\n");
    File configJson = LittleFS.open(APP_CONFIG_FILE_JSON, "w");
    uJson j = uJson(configJson);

    j.writeHeader();
    j.writeEntry(A_wifi_ssid, appcfgWR.wifi_ssid);
    j.writeEntry(A_wifi_password, appcfgWR.wifi_password);
    j.writeEntry(A_wifi_mode, appcfgWR.wifi_mode);

    j.writeEntry(A_net_mode, appcfgWR.net_mode);
    j.writeEntry(A_net_host, appcfgWR.net_host);
    j.writeEntry(A_net_mask, appcfgWR.net_mask);
    j.writeEntry(A_net_gateway, appcfgWR.net_gateway);
    j.writeEntry(A_net_dns, appcfgWR.net_dns);

    j.writeEntry(A_ota_enabled, appcfgWR.ota_enabled);
    j.writeEntry(A_ota_hostname, appcfgWR.ota_hostname);
    j.writeEntry(A_ota_password, appcfgWR.ota_password);

    j.writeEntry(A_admin_password, appcfgWR.admin_password);

    j.writeEntry(A_mqtt_enabled, appcfgWR.mqtt_enabled);
    j.writeEntry(A_mqtt_clientid, appcfgWR.mqtt_clientid);
    j.writeEntry(A_mqtt_host, appcfgWR.mqtt_host);
    j.writeEntry(A_mqtt_port, appcfgWR.mqtt_port);
    j.writeEntry(A_mqtt_intopic, appcfgWR.mqtt_intopic);
    j.writeEntry(A_mqtt_outtopic, appcfgWR.mqtt_outtopic);
    j.writeEntry(A_mqtt_useauth, appcfgWR.mqtt_useauth);
    j.writeEntry(A_mqtt_user, appcfgWR.mqtt_user);
    j.writeEntry(A_mqtt_password, appcfgWR.mqtt_password);
    j.writeEntry(A_mqtt_sending_interval, appcfgWR.mqtt_sending_interval);

    j.writeEntry(A_telnet_enabled, appcfgWR.telnet_enabled);

    j.writeEntry(A_ntp_enabled, appcfgWR.ntp_enabled);
    j.writeEntry(A_ntp_timezone, appcfgWR.ntp_timezone);
    j.writeEntry(A_ntp_server1, appcfgWR.ntp_server1);
    j.writeEntry(A_ntp_server2, appcfgWR.ntp_server2);
    j.writeEntry(A_ntp_server3, appcfgWR.ntp_server3);

    j.writeEntry(A_ping_enabled, appcfgWR.ping_enabled);
    j.writeEntry(A_ping_interval, appcfgWR.ping_interval);
    j.writeEntry(A_ping_ip_slot1, appcfgWR.ping_addr[0]);
    j.writeEntry(A_ping_ip_slot2, appcfgWR.ping_addr[1]);
    j.writeEntry(A_ping_ip_slot3, appcfgWR.ping_addr[2]);
    j.writeEntry(A_ping_ip_slot4, appcfgWR.ping_addr[3]);
    j.writeEntry(A_ping_ip_slot5, appcfgWR.ping_addr[4]);
    j.writeEntry(A_ping_ip_slot6, appcfgWR.ping_addr[5]);
    j.writeEntry(A_ping_ip_slot7, appcfgWR.ping_addr[6]);

    j.writeFooter();
    configJson.close();

    FSInfo fs_info;
    LittleFS.info(fs_info);

    fsTotalBytes = fs_info.totalBytes;
    fsUsedBytes = fs_info.usedBytes;

    Serial.printf("\n--- SPIFFS Info ---\ntotal bytes = %d\n",
                  fs_info.totalBytes);
    Serial.printf("used bytes = %d\n", fs_info.usedBytes);
    Serial.printf("block size = %d\n", fs_info.blockSize);
    Serial.printf("page size = %d\n", fs_info.pageSize);
    Serial.printf("max open files = %d\n", fs_info.maxOpenFiles);
    Serial.printf("max path length = %d\n", fs_info.maxPathLength);

    LittleFS.end();
  }
}

void App::printConfig(AppConfig ac)
{
  Serial.println();
  Serial.println("--- App appcfguration -----------------------------------");
  Serial.println("  Security:");
  Serial.printf("    Admin password: %s\n", ac.admin_password);
  Serial.println("\n  WiFi:");
  Serial.printf("    SSID: %s\n", ac.wifi_ssid);
  Serial.printf("    Password: %s\n", ac.wifi_password);
  Serial.printf("    Mode: %s\n",
                (ac.wifi_mode == 1) ? "Station" : "Access Point");
  Serial.println("\n  Network:");
  Serial.printf("    Mode: %s\n",
                (ac.net_mode == NET_MODE_DHCP) ? "DHCP" : "Static");
  Serial.printf("    host address: %s\n", ac.net_host);
  Serial.printf("    gateway: %s\n", ac.net_gateway);
  Serial.printf("    netmask: %s\n", ac.net_mask);
  Serial.printf("    dns server: %s\n", ac.net_dns);
  Serial.println("\n  OTA:");
  Serial.printf("    Enabled: %s\n", (ac.ota_enabled ? "true" : "false"));
  Serial.printf("    Hostname: %s\n", ac.ota_hostname);
  Serial.printf("    Password: %s\n", ac.ota_password);

  Serial.println("\n  MQTT:");
  Serial.printf("    Enabled: %s\n", (ac.mqtt_enabled ? "true" : "false"));
  Serial.printf("    Client ID: %s\n", ac.mqtt_clientid);
  Serial.printf("    Host: %s\n", ac.mqtt_host);
  Serial.printf("    Port: %d\n", ac.mqtt_port);
  Serial.printf("    Use Auth: %s\n", (ac.mqtt_useauth ? "true" : "false"));
  Serial.printf("    User: %s\n", ac.mqtt_user);
  Serial.printf("    Password: %s\n", ac.mqtt_password);
  Serial.printf("    In Topic: %s\n", ac.mqtt_intopic);
  Serial.printf("    Out Topic: %s\n", ac.mqtt_outtopic);
  Serial.printf("    Sending Interval: %ld\n", ac.mqtt_sending_interval);

  Serial.println("\n  Telnet:");
  Serial.printf("    Enabled: %s\n", (ac.telnet_enabled ? "true" : "false"));

  Serial.println("\n  NTP:");
  Serial.printf("    Enabled: %s\n", (ac.ntp_enabled ? "true" : "false"));
  Serial.printf("    Timezone: %s\n", ac.ntp_timezone);
  Serial.printf("    NTP Server 1: %s\n", ac.ntp_server1);
  Serial.printf("    NTP Server 2: %s\n", ac.ntp_server2);
  Serial.printf("    NTP Server 3: %s\n", ac.ntp_server3);
  
  Serial.println("\n  Ping:");
  Serial.printf("    Enabled: %s\n", (ac.ping_enabled ? "true" : "false"));
  Serial.printf("    Interval: %ld\n", ac.ping_interval);
  for( int i=0; i<7; i++ )
  {
    Serial.printf("    NTP Server %d: %s\n", (i+1), ac.ping_addr[i]);
  }

  Serial.println("---------------------------------------------------------");
  Serial.println();
}

void App::delayedSystemRestart()
{
  doSystemRestart = true;
  systemRestartTimestamp = millis();
  systemRestartCounter = 6;
  LOG0("*** delayedSystemRestart ***\n");
}

void App::handle(unsigned long timestamp)
{

  if (doSystemRestart && ((timestamp - systemRestartTimestamp) > 1000))
  {

    systemRestartCounter--;

    if (systemRestartCounter > 0)
    {
      LOG1("*** system restart in %us ***\n", systemRestartCounter);
    }
    else
    {
      LOG1("*** do system restart *** (%lu)\n", (timestamp - systemRestartTimestamp));
      writeConfig();
      restartSystem();
    }

    systemRestartTimestamp = millis();
  }
}

bool App::loadJsonConfig(const char *filename)
{
  bool readError = false;
  char attributeName[128];

  File tmpConfig = LittleFS.open(filename, "r");

  uJson j = uJson(tmpConfig);

  if (j.readHeader())
  {
    memcpy(&appcfgRD, &appcfg, sizeof(appcfg));

    while (readError == false && j.readAttributeName(attributeName) == true)
    {
      readError |= j.readEntryChars(attributeName, A_wifi_ssid, appcfgRD.wifi_ssid);
      readError |= j.readEntryChars(attributeName, A_wifi_password, appcfgRD.wifi_password);
      readError |= j.readEntryInteger(attributeName, A_wifi_mode, &appcfgRD.wifi_mode);

      readError |= j.readEntryInteger(attributeName, A_net_mode, &appcfgRD.net_mode);
      readError |= j.readEntryChars(attributeName, A_net_host, appcfgRD.net_host);
      readError |= j.readEntryChars(attributeName, A_net_mask, appcfgRD.net_mask);
      readError |= j.readEntryChars(attributeName, A_net_gateway, appcfgRD.net_gateway);
      readError |= j.readEntryChars(attributeName, A_net_dns, appcfgRD.net_dns);

      readError |= j.readEntryBoolean(attributeName, A_ota_enabled, &appcfgRD.ota_enabled);
      readError |= j.readEntryChars(attributeName, A_ota_hostname, appcfgRD.ota_hostname);
      readError |= j.readEntryChars(attributeName, A_ota_password, appcfgRD.ota_password);

      readError |= j.readEntryChars(attributeName, A_admin_password, appcfgRD.admin_password);

      readError |= j.readEntryBoolean(attributeName, A_mqtt_enabled, &appcfgRD.mqtt_enabled);
      readError |= j.readEntryChars(attributeName, A_mqtt_clientid, appcfgRD.mqtt_clientid);
      readError |= j.readEntryChars(attributeName, A_mqtt_host, appcfgRD.mqtt_host);
      readError |= j.readEntryInteger(attributeName, A_mqtt_port, &appcfgRD.mqtt_port);
      readError |= j.readEntryChars(attributeName, A_mqtt_intopic, appcfgRD.mqtt_intopic);
      readError |= j.readEntryChars(attributeName, A_mqtt_outtopic, appcfgRD.mqtt_outtopic);
      readError |= j.readEntryBoolean(attributeName, A_mqtt_useauth, &appcfgRD.mqtt_useauth);
      readError |= j.readEntryChars(attributeName, A_mqtt_user, appcfgRD.mqtt_user);
      readError |= j.readEntryChars(attributeName, A_mqtt_password, appcfgRD.mqtt_password);
      readError |= j.readEntryULong(attributeName, A_mqtt_sending_interval, &appcfgRD.mqtt_sending_interval);

      readError |= j.readEntryBoolean(attributeName, A_telnet_enabled, &appcfgRD.telnet_enabled);

      readError |= j.readEntryBoolean(attributeName, A_ntp_enabled, &appcfgRD.ntp_enabled);
      readError |= j.readEntryChars(attributeName, A_ntp_timezone, appcfgRD.ntp_timezone);
      readError |= j.readEntryChars(attributeName, A_ntp_server1, appcfgRD.ntp_server1);
      readError |= j.readEntryChars(attributeName, A_ntp_server2, appcfgRD.ntp_server2);
      readError |= j.readEntryChars(attributeName, A_ntp_server3, appcfgRD.ntp_server3);

      readError |= j.readEntryBoolean(attributeName, A_ping_enabled, &appcfgRD.ping_enabled);
      readError |= j.readEntryULong(attributeName, A_ping_interval, &appcfgRD.ping_interval);
      readError |= j.readEntryChars(attributeName, A_ping_ip_slot1, appcfgRD.ping_addr[0]);
      readError |= j.readEntryChars(attributeName, A_ping_ip_slot2, appcfgRD.ping_addr[1]);
      readError |= j.readEntryChars(attributeName, A_ping_ip_slot3, appcfgRD.ping_addr[2]);
      readError |= j.readEntryChars(attributeName, A_ping_ip_slot4, appcfgRD.ping_addr[3]);
      readError |= j.readEntryChars(attributeName, A_ping_ip_slot5, appcfgRD.ping_addr[4]);
      readError |= j.readEntryChars(attributeName, A_ping_ip_slot6, appcfgRD.ping_addr[5]);
      readError |= j.readEntryChars(attributeName, A_ping_ip_slot7, appcfgRD.ping_addr[6]);
    }
  }

  tmpConfig.close();

  return readError;
}

void App::wifiLedOn()
{
#ifdef WIFI_LED
  wifiLedState = 1;
  digitalWrite(WIFI_LED, WIFI_LED_ON);
  ledStateTimestamp = millis();
#endif
}

void App::wifiLedOff()
{
#ifdef WIFI_LED
  wifiLedState = 0;
  digitalWrite(WIFI_LED, WIFI_LED_OFF);
  ledStateTimestamp = millis();
#endif
}

void App::wifiLedToggle()
{
#ifdef WIFI_LED
  if (wifiLedState == 1)
  {
    wifiLedOff();
  }
  else
  {
    wifiLedOn();
  }
#endif
}

