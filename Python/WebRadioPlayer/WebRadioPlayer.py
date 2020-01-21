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


##########################################################################################################

RESET_PIN = digitalio.DigitalInOut(board.D4)


##########################################################################################################

#THE oled screen object
oled = None

# Load a font in 2 different sizes.
font = ImageFont.truetype('/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf', 20)
font2 = ImageFont.truetype('/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf', 14)

##########################################################################################################


def initScreen():
    # Very important... This lets py-gaugette 'know' what pins to use in order to reset the display
    global oled
    i2c = board.I2C()
    oled = adafruit_ssd1306.SSD1306_I2C(128, 32, i2c, addr=0x3c, reset=RESET_PIN)


    
def clearScreen():
    # Clear display.
    oled.fill(0)
    oled.show()
    

def showStartupScreen():
    msg = ["Web", "Radio", "Player"]
    for m in msg:
        # Create blank image for drawing.
        image = Image.new('1', (oled.width, oled.height))
        draw = ImageDraw.Draw(image)
        
        # Draw the text
        draw.text((0, 0), m, font=font, fill=255)
    
        # Display image
        oled.image(image)
        oled.show()

        #wait a little
        time.sleep(0.3)



            
################################################################################################3
##
##  Main entry point
##
################################################################################################3
if __name__ == '__main__':
    #inits
    initScreen()

    #startup
    clearScreen()

    showStartupScreen()

    time.sleep(2)

    clearScreen()

    
    
