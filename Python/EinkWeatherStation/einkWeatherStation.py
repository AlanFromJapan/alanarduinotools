
import epd2in13
import datetime
import time
import Image
import ImageDraw
import ImageFont
import weatherbitio as wbit
import traceback
import os
import config

#for buttons
import RPi.GPIO as GPIO

#some utils functions
import alan_utils
from designer import drawWeatherPanel, drawShutdownPanel, drawOthersPanel, drawEndPanel

################################################################################################3
##
##  Constants & Globals
##
################################################################################################3

#Tokyo
CITYCODE="1850147"
KEY=config.weatherio["key"]


# Pin Definitons: it's the GPIO##, not the Pin number on the connector (beware)
BUTTONPINA = 16
BUTTONPINB = 12

#LED : GPIO05 means PIN 29 on controller
LEDPIN = 5

#Thursday is #3 day of week (Monday == 0)
DOW_THURSDAY = 3

#the epd object for the display
epd = None

#last time weather was updated
lastWeatherDT = None

#available panels and current one
PANELS = ["Weather", "Shutdown", "Others"]
currentPanelIdx = 0

################################################################################################3
##
##  Functions
##
################################################################################################3



#Get the forecast every 3h for 24h
def getWeather(forecastIndex):
    w = wbit.getNext24hby3h(KEY, CITYCODE)["data"][forecastIndex]
    #print(w)
    return w

#Get CURRENT weather
def getWeatherNow():
    w = wbit.getCurrentWeather(KEY, CITYCODE)
    #print(w)
    return w



#Does the init for the eInk display
def eInkInit():
    epd = epd2in13.EPD()
    epd.init(epd.lut_full_update)

    epd.clear_frame_memory(0xFF)
    #    epd.set_frame_memory(image, 0, 0)
    #    epd.display_frame()
    #    epd.init(epd.lut_partial_update)
    return epd
    

#shows the image parameter on the eInk display
def eInkShow(epd, img):
    epd.set_frame_memory(img, 0, 0)
    epd.display_frame()
    #go to sleep to not damage the display
    epd.sleep()


#Init the GPIO buttons
def initButtons():
    GPIO.setmode(GPIO.BCM) # Broadcom pin-numbering scheme
    #Buttons setup
    GPIO.setup(BUTTONPINA, GPIO.IN, pull_up_down=GPIO.PUD_UP)
    GPIO.setup(BUTTONPINB, GPIO.IN, pull_up_down=GPIO.PUD_UP)
    GPIO.add_event_detect(BUTTONPINA, GPIO.FALLING, callback=buttonCallbackA, bouncetime=300)  
    GPIO.add_event_detect(BUTTONPINB, GPIO.FALLING, callback=buttonCallbackB, bouncetime=300)  

    #Now the LED
    GPIO.setup(LEDPIN, GPIO.OUT) # LED pin set as output
    # Initial state for LEDs:
    GPIO.output(LEDPIN, GPIO.LOW)


#Red button pressed : confirm action
def buttonCallbackA(channel):
    print ("DEBUG: Pressed [A]/[RED] !")

    #if shudown then do shutdown
    if PANELS[currentPanelIdx] == "Shutdown":
        print ("Good night!")
        try:
            #wake up! in case it sleeps
            epd.reset()
            
            img = drawEndPanel()
            img = img.rotate(90)
            eInkShow(epd, img)

        finally:
            os.system("sudo shutdown -h now")
        return


    
#Green button pressed : move to next panel
def buttonCallbackB(channel):
    print ("DEBUG: Pressed [B]/[GREEN] !")
    global currentPanelIdx
    currentPanelIdx = (currentPanelIdx + 1) % len(PANELS)

    #draws the UPDATED  panel
    drawCurrentPanel()


################################################################################################3
##
##  Choose which panel to draw
##
################################################################################################3
def drawCurrentPanel():
    #wake up! in case it sleeps
    epd.reset()

    try:
        if PANELS[currentPanelIdx] == "Weather":
            try:
                #get current weather
                wNow = getWeatherNow ()

                #now +6h-9h
                wLater = getWeather(2)

                #some consistency checks
                if wNow == None or wLater == None:
                    print ("**** Error: received None as weather forecast!")
                    raise ValueError ("Now or Later weather received was NULL (None).")

                img = drawWeatherPanel(wNow, wLater)
                img = img.rotate(90)
                eInkShow(epd, img)

            except BaseException,ex:
                #something wrong happened
                traceback.print_exc()
                #skip refresh this time
                return


        elif PANELS[currentPanelIdx] == "Shutdown":
            img = drawShutdownPanel()
            img = img.rotate(90)
            eInkShow(epd, img)

        elif PANELS[currentPanelIdx] == "Others":
            img = drawOthersPanel()
            img = img.rotate(90)
            eInkShow(epd, img)

        else:
            #what the hell are we doing here??
            raise ValueError ("Unknown current panel idx : %d" % (currentPanelIdx))
    finally:
        #go to sleep to not damage the display
        epd.sleep()
        
    
################################################################################################3
##
##  Main entry point
##
################################################################################################3
if __name__ == '__main__':
    #start on weather
    currentPanelIdx = 0

    #init the e-Ink
    epd = eInkInit()

    #init buttons
    initButtons()

    #draws the current panel
    drawCurrentPanel()

    print("Here we go! Press CTRL+C to exit")
    try:
        while 1:
            #print (".")
            time.sleep(1)
            now = datetime.datetime.now()

            #turn the pin low by default every round
            GPIO.output(LEDPIN, GPIO.LOW)

            #If weather do autorefresh
            if currentPanelIdx == 0 and not lastWeatherDT == None:
                tdelta = now - lastWeatherDT
                #refresh every 20 mins = 1200 sec
                if tdelta.total_seconds() > 1200:
                    print("Weather: force refresh.")
                    #force refresh
                    drawCurrentPanel()
                
            #Led should blink the 2nd Thrusday of the month
            #I SHOULD use futures or threads, I'm just being lazy and lucky since button change of panel is a callback on another thread (free multithreading)
            ajd = now.today()
            dom = ajd.day
            if ajd.weekday() == DOW_THURSDAY and (8 <= dom <= 14 or 22 <= dom <= 28):
                GPIO.output(LEDPIN, GPIO.LOW if now.second %2 ==0 else GPIO.HIGH)

    except KeyboardInterrupt: # If CTRL+C is pressed, exit cleanly:
        pass
    finally:
        #go to sleep to not damage the display
        epd.sleep()
        # cleanup all GPIO
        GPIO.cleanup() 
        print("Good bye.")
        
