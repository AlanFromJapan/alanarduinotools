

#for the screen
import board
import digitalio
from PIL import Image, ImageDraw, ImageFont
import adafruit_ssd1306

#misc imports
import config
import time
import os

import concurrent.futures

#THE class in charge of the screen design
class Designer:
    PLAYBOXW = 24
    PLAYBOXH = 24
    PATH_TO_IMG = "images/"

    #FONTPATH = '/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf'
    
    #########################################################################################

    # Load a font in 2 different sizes.
    fontBig = ImageFont.truetype("fonts/SUBWT.ttf", 24)
    fontSmall = ImageFont.truetype("fonts/DisposableDroidBB.ttf", 16)

    #the screen ref
    oled = None

    #multithreading executor
    executor = concurrent.futures.ThreadPoolExecutor(max_workers=1)
    fut = None
    
    #########################################################################################

    #Contructor
    def __init__(self, oled):
        self.oled = oled

        
    #clears the screen
    def clearScreen(self):
        # Clear display.
        self.oled.fill(0)
        self.oled.show()

        
    #poweroff the screen (the backend function)
    def powerOffScreen(self):
        time.sleep(10)
        self.oled.poweroff()


    #turn off the screen (the caller)
    def setScreenTimeout(self):
        if not bool(config.general["screenAutoOff"]):
            return
        #if already scheduled, kill and reschedule
        if self.fut is not None:
            self.fut.cancel()            
        self.fut = self.executor.submit(self.powerOffScreen())

        
    #wake up the screen
    def screenWakeup(self):
        if not bool(config.general["screenAutoOff"]):
            return
        if not self.oled.power:
            self.oled.poweron()
        

    #show a message on the screen
    def showMessage(self, m, sleep=0, clearAfter=False, font=fontSmall):
        #reset the screen
        self.screenWakeup()
        
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
    def showScreenLeftBox(self, label, boxtype="play"):
        #reset the screen
        self.screenWakeup()

        image = Image.new('1', (self.oled.width, self.oled.height))
        draw = ImageDraw.Draw(image)

        vpad= (self.oled.height-self.PLAYBOXH) /2
        draw.rectangle([(0,vpad), (self.PLAYBOXW-1, self.oled.height-1-vpad)], fill=255, outline=255)

        #the images apparently just work as a "mask": whatever is drawn, in black or white, becomes black on the screen...
        if boxtype == "play":
            bmp = Image.open (os.path.join(self.PATH_TO_IMG, "play_bt.png"))
        elif boxtype == "pause":
            bmp = Image.open (os.path.join(self.PATH_TO_IMG, "pause.png"))
        else:
            bmp = Image.open (os.path.join(self.PATH_TO_IMG, "settings.png"))
        draw.bitmap ( (0, vpad), bmp )
        
        # Draw the text        
        draw.text((self.PLAYBOXW + 2, vpad), label, font=self.fontSmall, fill=255)

        #Flip?
        if bool(config.general["flipScreen"]):
            image = image.rotate(180)
    
        # Display image
        self.oled.image(image)
        self.oled.show()


        
    #shows a "PLAY" screen with the radio name
    def showScreenPlay(self, radioName):
        self.showScreenLeftBox (label=radioName, boxtype="play")
        self.setScreenTimeout()

                
    #shows a "PAUSE" screen with the radio name
    def showScreenPause(self, radioName):
        self.showScreenLeftBox (label=radioName, boxtype="pause")
        self.setScreenTimeout()

        
    #shows a "STOP" screen to allow turn off machine
    def showScreenStop(self):
        self.showScreenLeftBox(label="Shudown?", boxtype="stop")

        
    #shows a "KILL" screen to allow to kill the current process
    def showScreenKill(self):
        self.showScreenLeftBox(label="Kill process?", boxtype="stop")
