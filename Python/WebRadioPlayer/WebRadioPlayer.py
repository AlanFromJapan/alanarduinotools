##########################################################################################################
##
## WebRadio player - http://electrogeek.cc/raspi%20webradio%20player.html
## Jan 2020
##
## Run me with Python3! 
##
##########################################################################################################

#for the screen
import board
import digitalio
from PIL import Image, ImageDraw, ImageFont
import adafruit_ssd1306

#for buttons
import RPi.GPIO as GPIO

#misc imports
import time
import os
import config
import subprocess
import sys

#display
import designer

#playa
import player

##########################################################################################################

RESET_PIN = digitalio.DigitalInOut(board.D4)

BUTTONA = 21
BUTTONB = 20

STATUS= ["play", "pause", "stop"]

##########################################################################################################

#THE oled screen object
oled = None

#THE designer
d = None

#THE player
p = None

#state machine status
mCurrentState = "pause"
mCurrentRadioName = None

##########################################################################################################

#init the screen
def initScreen():
    # Very important... This lets py-gaugette 'know' what pins to use in order to reset the display
    global oled, d
    i2c = board.I2C()
    oled = adafruit_ssd1306.SSD1306_I2C(128, 32, i2c, addr=0x3c, reset=RESET_PIN)
    d = designer.Designer (oled)




        
#Init the GPIO buttons
def initButtons():
    GPIO.setmode(GPIO.BCM) # Broadcom pin-numbering scheme
    #Buttons setup
    GPIO.setup(BUTTONA, GPIO.IN, pull_up_down=GPIO.PUD_UP)
    GPIO.setup(BUTTONB, GPIO.IN, pull_up_down=GPIO.PUD_UP)
    GPIO.add_event_detect(BUTTONA, GPIO.FALLING, callback=buttonCallbackA, bouncetime=500)  
    GPIO.add_event_detect(BUTTONB, GPIO.FALLING, callback=buttonCallbackB, bouncetime=500)  



#White button pressed : Play/Pause/Valid
def buttonCallbackA(channel):
    global mCurrentState
    print ("DEBUG: Pressed [A]/[WHITE] !")

    if mCurrentState == "play":
        mCurrentState = "pause"
    elif mCurrentState == "pause":
        mCurrentState = "play"
    elif mCurrentState == "stop":
        if bool(config.general["shutdownOnlyExit"]):
            oled.poweroff()
            sys.exit()
        else:
            oled.poweroff()
            subprocess.Popen (["sudo", "shutdown", "-h", "now"])

    updatePlayerStatus()
    
    #update the screen
    showCurrentScreen()


    
#White button pressed : change channel
def buttonCallbackB(channel):
    global mCurrentRadioName, mCurrentState
    print ("DEBUG: Pressed [B]/[YELLOW] !")

    if mCurrentState in ("play", "pause"):
        l = list(config.radios.keys())
        i = l.index(mCurrentRadioName)
        i = (i + 1) % len(l)
        mCurrentRadioName = l[i]

        #in case, pause playing
        mCurrentState = "pause"

        updatePlayerStatus()

        #we did a full cycle through channels back to the first one: squeeze in the options menu
        if i == 0:
            #go to settings
            mCurrentState = "stop"
    elif mCurrentState == "stop":
        #in case, pause playing
        mCurrentState = "pause"
            
    #update the screen
    showCurrentScreen()
    



#Show the current screen as per the current status
def showCurrentScreen():
    if mCurrentState == "play":
        d.showScreenPlay(mCurrentRadioName)
    elif mCurrentState == "pause":
        d.showScreenPause(mCurrentRadioName)
    elif mCurrentState == "stop":
        d.showScreenStop()


def updatePlayerStatus():
    global mCurrentState
    try:
        if mCurrentState == "play":
            p.play(config.radios[mCurrentRadioName])
        elif mCurrentState == "pause":
            p.pause()
    except BaseException as e:
        print(str(e))
        d.showMessage(str(e), font=d.fontSmall, sleep=3)
        #go back or stay in pause
        mCurrentState = "pause"
        showCurrentScreen()
            
        
    
    
################################################################################################3
##
##  Main entry point
##
################################################################################################3
if __name__ == '__main__':
    try:
        #inits
        initScreen()
        initButtons()

        p = player.Player()
    
        #startup
        d.clearScreen()

        d.showStartupScreen()
        
        d.clearScreen()

        #show how many radios
        m = str(len(config.radios)) + " radios registered."
        d.showMessage(m, font=d.fontSmall, sleep=1)

        #play the first radio
        #below line : make a list() before taking index 0 because dict_keys() is a set (there's no "order")
        mCurrentRadioName = list(config.radios.keys())[0]

        #Goto main screen
        updatePlayerStatus()
        showCurrentScreen()

        try:
            while (True):
                pass

        except KeyboardInterrupt: # If CTRL+C is pressed, exit cleanly:
            pass
        finally:
            # cleanup all GPIO
            GPIO.cleanup() 
            print("Good bye.")
    finally:
        oled.poweroff()

            
