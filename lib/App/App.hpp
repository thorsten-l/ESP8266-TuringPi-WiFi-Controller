#ifndef __APP_H__
#define __APP_H__

#include <Arduino.h>
#include <FS.h>
#include <DeviceConfig.hpp>
#include <ConfigAttributes.hpp>

#define LOG0( format ) Serial.printf( "(%lu) " format, millis())
#define LOG1( format, ... ) Serial.printf( "(%lu) " format, millis(), ##__VA_ARGS__ )

#define APP_NAME "Turing Pi WiFi Controller"
#define APP_AUTHOR "Dr. Thorsten Ludewig <t.ludewig@gmail.com>"
#define APP_CONFIG_FILE_JSON "/config.json"

// Network mode
#define NET_MODE_STATIC 1
#define NET_MODE_DHCP 2

extern void appShowHeader(Stream &out);
extern const char *appUptime();
extern const char *appDateTime();
extern void logMessage(const char *message);
extern void logMessage(String format, ...);

typedef struct appconfig
{
  char wifi_ssid[64];
  char wifi_password[64];
  int  wifi_mode;

  int net_mode;
  char net_host[64];
  char net_mask[64];
  char net_gateway[64];
  char net_dns[64];

  bool ota_enabled;
  char ota_hostname[64];
  char ota_password[64];

  char admin_password[64];

  bool mqtt_enabled;
  char mqtt_clientid[64];
  char mqtt_host[64];
  int  mqtt_port;
  bool mqtt_useauth;
  char mqtt_user[64];
  char mqtt_password[64];
  char mqtt_intopic[64];
  char mqtt_outtopic[64];
  unsigned long mqtt_sending_interval;

  bool telnet_enabled;

  bool ntp_enabled;
  char ntp_timezone[64];
  char ntp_server1[64];
  char ntp_server2[64];
  char ntp_server3[64];

  unsigned long ping_interval;
  char ping_addr[7][32];

} AppConfig;

class App
{
private:
  char initFilename[32];
  bool initialized = false;
  bool doSystemRestart;
  unsigned long systemRestartTimestamp;
  unsigned int systemRestartCounter;
  bool initSPIFFS = false;
  unsigned long ledStateTimestamp;
  int wifiLedState;

  void formatSPIFFS();
  void loadConfig();
  void restartSystem();

public:
  size_t fsTotalBytes;
  size_t fsUsedBytes;

  App();

  void setup();
  void firmwareReset();
  void defaultConfig();
  void writeConfig();
  bool loadJsonConfig( const char *filename );
  void printConfig(AppConfig ac);
  void delayedSystemRestart();
  void handle( unsigned long timestamp );
  //
  void showLeds();
  void wifiLedOn();
  void wifiLedOff();
  void wifiLedToggle();
};

extern App app;
extern AppConfig appcfg;
extern AppConfig appcfgWR;
extern AppConfig appcfgRD;

#endif
