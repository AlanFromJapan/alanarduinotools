# USB Mini Media Keyboard / Smartphone Media keyboard

A small USB-C device you plug to your phone and registers as a keyboard and can **Play/Pause** or **Next** your music on your smartphone.

Tested on Android smartphone, works fine.

**See details on my website https://electrogeek.cc/smartphone%20media%20keyboard.html**

# Technicalities

## Board 

Kicad schematics here [https://github.com/AlanFromJapan/pcb-design/tree/master/SmartphoneMediaKeyboard]

- Need a ATtiny2313 (DIP-20 model)
- Need a 12 MHz crystal
- Need a 5.1k (4.7k works) resistor for setting device as USB-C slave

## Fuses

Program with `avrdude -pt2313 -cavrispmkII -u  -Ulfuse:w:0xce:m -Uhfuse:w:0xdf:m -Uefuse:w:0xff:m`

