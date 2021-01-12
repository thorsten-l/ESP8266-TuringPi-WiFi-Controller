#!/bin/sh
cd ..
docker run -it --rm \
  -v `pwd`:/workdir \
  --name platformio platformio \
  /usr/local/bin/platformio run \
    -e d1_mini \
    -e d1_mini_lite \
    -e d1_mini_pro \
    -e d1_mini_2m

cp .pio/build/*/firmware-*.bin firmware
gzip -9 firmware/*.bin
