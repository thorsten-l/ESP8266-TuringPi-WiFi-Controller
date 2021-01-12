 #!/bin/sh
 $HOME/.platformio/packages/tool-esptool/esptool -cd ck -cb 115200 \
   -cp /dev/cu.SLAB_USBtoUART -cf firmware.bin
