#!/bin/sh
$HOME/.platformio/penv/bin/python \
  $HOME/.platformio/packages/framework-arduinoespressif8266/tools/espota.py \
  --debug --progress -i "tpictl.local" \
  --auth="otapass" -f "firmware.bin"
