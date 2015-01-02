import time
import struct

f = open(r'\\.\pipe\TricolorUsbSignalManagerIn', 'r+b', 0)
i = 1

while True:
    s = "hello monde\r\n"
    f.write(bytes(s, 'UTF-8'))
    f.seek(0)
    
##    f.write(struct.pack('I', 70))   # Write str length and str
##    f.seek(0)                               # EDIT: This is also necessary
##    f.write(struct.pack('I', 13))   # Write str length and str
##    f.seek(0)                               # EDIT: This is also necessary
##    f.write(struct.pack('I', 10))   # Write str length and str
##    f.seek(0)                               # EDIT: This is also necessary
    
    print("Sent data")
    time.sleep(2)
