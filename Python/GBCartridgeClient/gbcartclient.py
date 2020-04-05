# pip3 install pyserial
import serial 
import sys
import os
import time

#length of the ROM to read in Bytes
#READROM_LEN = 32768
READROM_LEN = 256

def readser():
    reps = ser.readlines()
    for rep in reps:
        print(rep.decode(), end='')


def prompt():
    print("Type a raw command, '>filename' to READ, '<filename' to WRITE, Q to quit:")


def readRom(p):
    print ("Read %dB ROM to '%s'" % (READROM_LEN, p) )
    cmd = 'l' + hex(READROM_LEN)[2:].zfill(4)  #skip the '0x', pas left with zeroes to be 4 char long always
    #print ("DBG: cmd = " + cmd)
    ser.write(bytearray(cmd, 'utf8')) #read 32k
    rep = None
    
    while True:
        r = ser.read(99)
        if rep == None:
            rep = r
        else:
            rep = rep + r
        time.sleep(1)
        if ser.in_waiting == 0:
            break

    print("... received data, starting to write to file...")
    i = 0
    with open(p, "wb") as t:
        while i < READROM_LEN:
            v = rep[i*3+1:i*3+3] #v has the hex value 
            b = int(v, 16)
            print("DBG %d : %s -> %d" % (i, v, b))
            #print (bytearray([b]))            
            t.write(bytearray([b]))
            i = i + 1

    print("Read rom finished.")



def writeRom(p):
    print ("TODO")



#default 9600,8,N,1 and 3sec timeout
ser = serial.Serial('/dev/ttyUSB0', timeout=3)  # open serial port
try:
    print(ser.name)         # check which port was really used
    ser.write(b'?')     # write a string
    readser()

    while (True):
        prompt()
        i = input("Command:")
        if i[0] == "<" or i[0] == ">":
            if i[0] == ">":
                readRom(i[1:])
            else:
                writeRom(i[1:])
        elif i[0] == "Q":
            sys.exit()
        elif i[0] == "!":
            print("DBG runs '>toto.gb'")
            readRom("toto.gb")
        else:
            ser.write(str.encode(i))
            
        readser()
        print()


finally :
    ser.close()             # close port
