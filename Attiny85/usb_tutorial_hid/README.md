This is the source code package for AVR ATtiny USB HID Tutorial by Joonas Pihlajamaa, published at Code and Life blog, http://codeandlife.com

http://codeandlife.com/2012/02/11/v-usb-tutorial-continued-hid-mouse/

The subfolder contains a part of V-USB library available at http://www.obdev.at/avrusb/ and its contents are copyrighted by its respective authors. My productions in the root folder are published under GNU GPL v3 (see License.txt).

I hope you have fun with this project!

- Joonas Pihlajamaa

# Edit from AlanFromJapan
AVI20240403 : took Joonas code and ported it to my ATtiny85 board just to have a something that works on my board (https://github.com/AlanFromJapan/pcb-design/tree/master/RapideUSBMini). 

Thanks to him for sharing 💗!
## Changes
- Upgraded the version used of V-USB to latest (at least I think so)
- Made an eclipse project (Makefile is ignored)
- Edited the usbdrv/usbconfig.h to reflect my board setup

## Notes
This RapideUSBMini can't be programmed if plugged to a real USB plug (D+/D- used or pulled low/high) so use a "USB condom" or a very dumb charger, or use aligator clips.
