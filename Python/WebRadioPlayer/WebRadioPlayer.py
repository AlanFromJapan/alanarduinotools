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


##########################################################################################################

RESET_PIN = digitalio.DigitalInOut(board.D4)

BUTTONA = 21
BUTTONB = 20

##########################################################################################################

#THE oled screen object
oled = None

# Load a font in 2 different sizes.
font1 = ImageFont.truetype('/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf', 24)
font2 = ImageFont.truetype('/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf', 12)

##########################################################################################################

#init the screen
def initScreen():
    # Very important... This lets py-gaugette 'know' what pins to use in order to reset the display
    global oled
    i2c = board.I2C()
    oled = adafruit_ssd1306.SSD1306_I2C(128, 32, i2c, addr=0x3c, reset=RESET_PIN)


#clears the screen
def clearScreen():
    # Clear display.
    oled.fill(0)
    oled.show()
    

#show a message on the screen
def showMessage(m, sleep=0, clearAfter=False, font=font2):
    image = Image.new('1', (oled.width, oled.height))
    draw = ImageDraw.Draw(image)
    
    # Draw the text
    draw.text((0, 0), m, font=font, fill=255)
    
    # Display image
    oled.image(image)
    oled.show()
    
    #wait a little?
    if sleep > 0:
        time.sleep(sleep)
    #clear screen?
    if clearAfter:
        clearScreen()
        

#makes the startup animation
def showStartupScreen():
    msg = ["Web", "Radio", "Player"]
    for m in msg:
        showMessage(m, sleep=0.3, clearAfter=True, font=font1)


        
#Init the GPIO buttons
def initButtons():
    GPIO.setmode(GPIO.BCM) # Broadcom pin-numbering scheme
    #Buttons setup
    GPIO.setup(BUTTONA, GPIO.IN, pull_up_down=GPIO.PUD_UP)
    GPIO.setup(BUTTONB, GPIO.IN, pull_up_down=GPIO.PUD_UP)
    GPIO.add_event_detect(BUTTONA, GPIO.FALLING, callback=buttonCallbackA, bouncetime=500)  
    GPIO.add_event_detect(BUTTONB, GPIO.FALLING, callback=buttonCallbackB, bouncetime=500)  



#Red button pressed : confirm action
def buttonCallbackA(channel):
    print ("DEBUG: Pressed [A]/[WHITE] !")
    showMessage("white")


    
#Green button pressed : move to next panel
def buttonCallbackB(channel):
    print ("DEBUG: Pressed [B]/[YELLOW] !")
    showMessage("yellow")


    
################################################################################################3
##
##  Main entry point
##
################################################################################################3
if __name__ == '__main__':
    #inits
    initScreen()
    initButtons()

    #startup
    clearScreen()

    showStartupScreen()

    clearScreen()

    m = str(len(config.radios)) + " radios registered."
    showMessage(m, font=font2)

    
    try:
        while (True):
            time.sleep(1)
            
    except KeyboardInterrupt: # If CTRL+C is pressed, exit cleanly:
        pass
    finally:
        # cleanup all GPIO
        GPIO.cleanup() 
        print("Good bye.")
        
    
