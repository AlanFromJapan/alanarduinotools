"""
V-USB name generator by AlanFromJapan
20150719 - http://kalshagar.wikispaces.com/
Input "hello"
Output 'h', 'e', 'l', 'l', 'o' (5 characters)
"""

import sys

if len(sys.argv) == 1:
    print("""V-USB name generator
--------------------
Easy way to generate the painful V-USB vendor name (USB_CFG_VENDOR_NAME) or Device name (USB_CFG_DEVICE_NAME)

  Usage: python %s hello
  Output: 'h', 'e', 'l', 'l', 'o' (5 characters)""" % (str(sys.argv[0])))
    sys.exit(1)
    
s = str(sys.argv[1])
print ("Input '%s'"  % (s))
first=True
for c in s:
    if first:
        first=False
    else:
        print (",", end="")
    print ("'%s'" % (str(c)), end="")
print ("") #new line
print ("Total: %s chars."% (str(len(s))))
