# CHANGES

## release 2.8.2

- LED night mode, switch off all leds after a specified timeout, for Robert :-)

## release 2.8.1

- root page state is more responsive

## release 2.8.0

- captive portal implementation
- captive portal DNS server for AP mode
- OpenHAB callback performance improvements
- check for invalid firmware filename implemented
- copy firmware.bin to firmware-version.pioenv.bin
- refresh to root page after 30s
- implementation for Shelly 1 OS switch

## release 2.7.0

- simple favicon added
- reorder openhab settings
- show millis as comment in setup page
- relpace all inline strings with define macros
- detailed WiFi info added to /state restful info
- new functions for backup and restore
- remove maintenance from setup page, now it has an own page
- Using JSON file format to store config

## release 2.6.0

- Implementation for BlitzWolf SHP6 inclusive energy monitor
- platform updated to version espressif8266@2.2.0
- own board definitions
  
## release 2.5.0

- reset firmware to system defaults added in setup page
- paramBool function moved to WebUtils

## release 2.4.2

- show chipinfo added
- new dns setting
- platform version 2.0.4
- initial travis configuration
- additional vscode exceptions

## release 2.4.1

- Pin initialization for development version was missing

## release 2.4.0

- html redesign of the setup page

## release 2.3.1

- show PIOENV_NAME in firmware upload label

## release 2.3.0

- update firmware over HTTP in setup page
- optimized platformio file structure
- web pages (handler) are now in seperate files
- added PIOENV_NAME to every platformio env
- show pioenv_name, hostname, spiffs_total and spiffs_used in info json

## release 2.2.2

- debug code removed
- remove unused code

## release 2.2.1

- using CPP macro for WIFI_LED ON / OFF
- complete rewriting of the setup page code
- dev1lite added

## release 2.1.0

- remove development firmware
- easier wifi network seletion, firmware is now available for both obi socket versions
- more information in info call
- show free heap in /info restful call

## release 2.0.6

- syslog messages added
- syslog enabled added to info page
- network and syslog added printConfig
- flashing esp info
- new screenshots for docs
- wrong default mqtt port in readme
- wrong default syslog port in readme

## release 2.0.5

- code modifications for fauxmoesp lib 3.1.0 (alexa)
- static network config and syslog config added
