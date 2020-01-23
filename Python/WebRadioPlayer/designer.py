

#for the screen
import board
import digitalio
from PIL import Image, ImageDraw, ImageFont
import adafruit_ssd1306

#misc imports
import config
import time
import os

#THE class in charge of the screen design
class Designer:
    PLAYBOXW = 24
    PLAYBOXH = 24
    PATH_TO_IMG = "images/"
    
    #########################################################################################

    # Load a font in 2 different sizes.
    fontBig = ImageFont.truetype('/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf', 24)
    fontSmall = ImageFont.truetype('/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf', 12)

    #the screen ref
    oled = None
    
    #########################################################################################

    #Contructor
    def __init__(self, oled):
        self.oled = oled
        
    #clears the screen
    def clearScreen(self):
        # Clear display.
        self.oled.fill(0)
        self.oled.show()
    

    #show a message on the screen
    def showMessage(self, m, sleep=0, clearAfter=False, font=fontSmall):
        image = Image.new('1', (self.oled.width, self.oled.height))
        draw = ImageDraw.Draw(image)
    
        # Draw the text
        draw.text((0, 0), m, font=font, fill=255)
        
        #Flip?
        if bool(config.general["flipScreen"]):
            image = image.rotate(180)
        
        # Display image
        self.oled.image(image)
        self.oled.show()
    
        #wait a little?
        if sleep > 0:
            time.sleep(sleep)
        #clear screen?
        if clearAfter:
            self.clearScreen()
        

    #makes the startup animation
    def showStartupScreen(self):
        msg = ["Web", "Radio", "Player"]
        for m in msg:
            self.showMessage(m, sleep=0.3, clearAfter=True, font=self.fontBig)


    #shows a "PLAY" or "PAUSE" screen with the radio name
    def showScreenPlayPause(self, radioName, isPlay=True):
        image = Image.new('1', (self.oled.width, self.oled.height))
        draw = ImageDraw.Draw(image)

        vpad= (self.oled.height-self.PLAYBOXH) /2
        draw.rectangle([(0,vpad), (self.PLAYBOXW-1, self.oled.height-1-vpad)], fill=255, outline=255)

        #the images apparently just work as a "mask": whatever is drawn, in black or white, becomes black on the screen...
        if isPlay:
            bmp = Image.open (os.path.join(self.PATH_TO_IMG, "play_bt.png"))
        else:
            bmp = Image.open (os.path.join(self.PATH_TO_IMG, "pause.png"))
        draw.bitmap ( (0, vpad), bmp )
        
        # Draw the text        
        draw.text((self.PLAYBOXW + 2, vpad), radioName, font=self.fontSmall, fill=255)

        #Flip?
        if bool(config.general["flipScreen"]):
            image = image.rotate(180)
    
        # Display image
        self.oled.image(image)
        self.oled.show()


        
    #shows a "PLAY" screen with the radio name
    def showScreenPlay(self, radioName):
        self.showScreenPlayPause (radioName, isPlay=True)

                
    #shows a "PAUSE" screen with the radio name
    def showScreenPause(self, radioName):
        self.showScreenPlayPause (radioName, isPlay=False)

        
