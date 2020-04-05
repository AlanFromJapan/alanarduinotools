# pip3 install pyserial
import serial 
import sys
import os
import time

#length of the ROM to read in Bytes
READROM_LEN = 32768

#used for debug, set it to a high value to not bug you
WRITEROM_MAXLEN = 32768

#should the write ROM to EEPROM reset it?
WRITEROM_RESETROMBEFORE = True


def readser():
    reps = ser.readlines()
    for rep in reps:
        print(rep.decode(), end='')


def prompt():
    print("Type a raw command, '>filename' to READ, '<filename' to WRITE, Q to quit:")


#READS a ROM from the EEPROM and saves it to disk
def readRom(p):
    print ("Read {0:,}B ROM to '{1}'".format(READROM_LEN, p) )
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
        time.sleep(0.1)
        if ser.in_waiting == 0:
            break

    print("... received data, starting to write to file...")
    i = 0
    with open(p, "wb") as f:
        while i < READROM_LEN:
            v = rep[i*3+1:i*3+3] #v has the hex value 
            b = int(v, 16)
            #print("DBG %d : %s -> %d" % (i, v, b))
            f.write(bytearray([b])) #must put the value in an array
            i = i + 1

    print("Read rom finished.")



#Writes a rom from disk to EEPROM (slowly)
def writeRom(p):
    print ("Starting to write ROM '%s'" % (p))

    #FIRST: you should have done an EEPROM reset    
    if WRITEROM_RESETROMBEFORE:
        print ("Reset ROM before writing.")
        ser.write(b'Z')
        #need to pause a bit, the erase takes 1-2 sec so be large
        time.sleep(5)
        print(ser.read(30).decode(), end="")
        

    #SECOND: place yourself at the start
    print ("Set cursor to EEPROM position 0")
    ser.write(b's0000')
    time.sleep(0.5)
    print(ser.read(30).decode(), end="")
    
    #LASTLY: start writing
    print("Starting writing!")
    with open(p, "rb") as f:
        i = 0
        while i < WRITEROM_MAXLEN: 
            buf = f.read(1)
            if buf == None:
                break

            cmd = "S" + hex(int(buf[0]))[2:].zfill(2).upper()
            #print("DBG: [%s] %s -> %s " % (str(i).zfill(4), cmd, buf))
            
            ser.write(bytearray(cmd, 'utf8'))
            time.sleep(0.05)
            print(ser.read(30).decode(), end="")
            time.sleep(0.01)

            i = i+1




# Does the command line prompt
def commandLineUI():
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
        elif i[0] == "#":
            print("DBG runs '<MysticSnake-n.gb'")
            writeRom("MysticSnake-n.gb")            
        else:
            ser.write(str.encode(i))
            
        readser()
        print()



#default 9600,8,N,1 and 2sec timeout
ser = serial.Serial('/dev/ttyUSB0', timeout=2)  # open serial port
try:
    print("Connected to " + ser.name)         # check which port was really used

    #was there a command line parameter for testing?
    if len(sys.argv) == 2:
        param = str(sys.argv[1])
        print ("Received parameter '%s'" % (param))
        if param == "testread":
            print("DBG runs '>toto.gb'")
            readRom("toto.gb")
        elif param == "testwrite":
            print("DBG runs '<MysticSnake-n.gb'")
            writeRom("MysticSnake-n.gb")    
        else:
            print("Dunno what to do, showing prompt")
        
    #normal mode
    commandLineUI()

finally :
    ser.close()             # close port
