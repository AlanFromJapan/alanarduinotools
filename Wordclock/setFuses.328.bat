avrdude -v -v -p m328p -c avrispv2 -P COM1 -U lfuse:w:0xf7:m -U hfuse:w:0xd9:m -U efuse:w:0xff:m
pause