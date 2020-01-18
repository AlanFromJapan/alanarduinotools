
#import epd2in13
import datetime
import time
from PIL import Image
from PIL import ImageDraw
from PIL import ImageFont
import traceback
import os
import config


#some utils functions
import alan_utils

################################################################################################3
##
##  Constants & Globals
##
################################################################################################3

PADDING = 5
PATH_TO_FONTS = "fonts/"
PATH_TO_ICONS = "retinaweather/"
PATH_TO_IMG = "images/"

#key is LOWERCASE!
condition2image = {
    "cloudy": "retina-cloud.png",    
    "cloudy-night": "retina-night-cloud.png",    

    "sunny" : "retina-sunny.png",
    "sunny-night" : "retina-night-clear.png",

    "rain" : "retina-rain.png",
    "rain-night" : "retina-rain.png",

    "snow" : "retina-snow.png",
    "snow-night" : "retina-snow.png",

    "thunder" : "retina-thunder.png",
    "thunder-night" : "retina-thunder.png",

    "mist": "retina-mist.png",
    "mist-night": "retina-mist.png",

    "drizzle": "retina-rain.png",
    "drizzle-night": "retina-rain.png",
    
}

#Fonts
font_xbig = ImageFont.truetype(os.path.join(PATH_TO_FONTS, 'DisposableDroidBB.ttf'), 48)
font_big = ImageFont.truetype(os.path.join(PATH_TO_FONTS, 'DisposableDroidBB.ttf'), 30)
font_medium = ImageFont.truetype(os.path.join(PATH_TO_FONTS, 'DisposableDroidBB.ttf'), 20)
font_mediumsmall = ImageFont.truetype(os.path.join(PATH_TO_FONTS, 'DisposableDroidBB.ttf'), 18)
font_small = ImageFont.truetype(os.path.join(PATH_TO_FONTS, 'DisposableDroidBB.ttf'), 16)
font_xsmall = ImageFont.truetype(os.path.join(PATH_TO_FONTS, 'DisposableDroidBB.ttf'), 9)

################################################################################################3
##
##  Functions
##
################################################################################################3


#returns the image to used based on condition keyword and time
def getImageFromCondition(condition, when):

    #print("DEBUG: when = %s" % (when))
    
    c = condition
    if when.hour < 8 or when.hour >= 20:
        c = c +  "-night"
        
    #print("DEBUG: search image for '%s' at %s." % (c, when))
    if c in condition2image: 
        return condition2image[c]
    else:
        return "unknown.png"



#make a blank image sized for the panel
def makeBlankPanelImage():
    # Display resolution in epd2in13.py
    EPD_WIDTH       = 128
    EPD_HEIGHT      = 250

    # For simplicity, the arguments are explicit numerical coordinates
    #image = Image.new('1', (epd2in13.EPD_HEIGHT, epd2in13.EPD_WIDTH), 255)  # 255: clear the frame
    
    image = Image.new('L', (EPD_HEIGHT, EPD_WIDTH), 255)  # 255: clear the frame
    draw = ImageDraw.Draw(image)

    #Screen is horizontal
    #the TOP LEFT (sticker)  angle is 0,0, BOTTOM RIGHT is max 
    image_width, image_height  = image.size

    return image, draw, image_width, image_height



#Returns the image from the Weather data
def getWeatherImg (w):
    try:
        #get the image to display
        imgName = "unknown.png" if w == None else getImageFromCondition(w["status"].lower(), w["datetimeobj"])
        
        return Image.open (os.path.join(PATH_TO_ICONS,imgName))
    except BaseException,ex:
        #failed to find the image most likey
        traceback.print_exc()
        return None
    


    
################################################################################################3
##
##  Draw Weather Panel (NEW)
##
################################################################################################3
def drawWeatherPanel(wNow, wLater):
    global lastWeatherDT
    global image_width
    global image_height
    
    lastWeatherDT = datetime.datetime.now()
    print("Draw weather panel : start at %s" % (lastWeatherDT.strftime("%Y/%m/%d %H:%M")))
    
    #make blank image and get all we need to draw
    image, draw, image_width, image_height = makeBlankPanelImage()

    imgWeather = getWeatherImg(wNow)

    ##########################################3
    ##
    ## DRAWING START
    ##
    #horizontal line
    for i in range(5):
        draw.line ([(0, 64+i),( image_width-64 -2*i, 64+i )], fill=0)


    ###########################################3
    ##
    ## TOP PART : WEATHER
    ##
    #the weather in text
    draw.text((PADDING, PADDING), wNow["weather"], font = font_big, fill = 0)
    #weather change
    vWeatherChange = "?"
    if not wLater == None:
        if wNow["status"] == wLater["status"]:
            vWeatherChange = "holding"
        else:
            vWeatherChange = wLater["weather"]
    vWeatherChange = "%s %s (%dc)" % (wLater ["time"][11:16], vWeatherChange, wLater["temp"])
    #temp min-max
    draw.text((PADDING, PADDING + 30), vWeatherChange, font = font_mediumsmall, fill = 0)


    ###########################################3
    ##
    ## BOTTOM PART : Misc info
    ##
    now = lastWeatherDT
    draw.text((PADDING, PADDING +60+10), now.strftime("%A"), font = font_medium, fill = 0)
    draw.text((PADDING, PADDING +60+30), now.strftime("%Y/%m/%d"), font = font_small, fill = 0)
    draw.text((PADDING, image_height - 14), now.strftime("Last update  %H:%M"), font = font_xsmall, fill = 0)

    #draw image
    draw.bitmap ( (image_width - 64 - PADDING, 64), imgWeather)

    #draw temp
    draw.text((image_width - 64 - PADDING - 48 -10, 64 + 8), "%2d" % (float(wNow["temp"])), font = font_xbig, fill = 0)
    draw.arc ([(image_width - 64 - 10 -10 , 64 + 12), (image_width - 64 -10 + 8 -10, 64 + 12 + 8)], 0, 360, fill= 0)
    

    # Moon
    #drawMoon()
        
    ###########################################3
    ##
    ## Drawing finished - display
    ##
    #img = image.rotate(90)

    #display on e-Ink
    #eInkShow(epd, img)
    return image



################################################################################################3
##
##  Draw Shutdown Panel
##
################################################################################################3
def drawShutdownPanel():
    print("DEBUG: shut panel")
    #make blank image and get all we need to draw
    image, draw, image_width, image_height = makeBlankPanelImage()

    draw.text (( 12, 24 ), "Shutdown?", font= font_xbig, fill = 0  )  
    draw.text (( 12, 64 ), "Press [RED] button to confirm", font= font_small, fill = 0  )  

    ## Drawing finished - display
    #img = image.rotate(90)
    #display on e-Ink
    #eInkShow(epd, img)
    return image

    
################################################################################################3
##
##  Draw Others Panel
##
################################################################################################3
def drawOthersPanel():
    print("DEBUG: others panel")
    #make blank image and get all we need to draw
    image, draw, image_width, image_height = makeBlankPanelImage()

    draw.text (( 12, 24 ), "Others...", font= font_xbig, fill = 0  )  
    draw.text (( 12, 64 ), "Current IP: %s" % (alan_utils.getWifiIP()), font= font_small, fill = 0  )  

    ## Drawing finished - display
    #img = image.rotate(90)
    #display on e-Ink
    #eInkShow(epd, img)
    return image


################################################################################################3
##
##  Draw The End panel
##
################################################################################################3
def drawEndPanel():
    print("DEBUG: End panel")
    #make blank image and get all we need to draw
    image, draw, image_width, image_height = makeBlankPanelImage()

    theend = Image.open (os.path.join(PATH_TO_IMG, "theend.png"))

    draw.bitmap ( (0, 0), theend )

    ## Drawing finished - display
    #img = image.rotate(90)
    #display on e-Ink
    #eInkShow(epd, img)
    return image



################################################################################################3
##
## In case of error
##
################################################################################################3
def drawErrorGeneric(ex):
    message = str(ex)
    print("DEBUG: generic error panel with message '%s'." % (message))
    #make blank image and get all we need to draw
    image, draw, image_width, image_height = makeBlankPanelImage()

    draw.text (( 0, 0 ), "Error:", font= font_small, fill = 0  )
    
    #    draw.text (( 0, 16 ), str(message), font= font_small, fill = 0  )
    n= 30
    splited = [message[i:i+n] for i in range(0, len(message), n)]

    y= deltay = 16
    for line in splited:
        draw.text (( 0, y ), line, font= font_small, fill = 0  )
        y= y + deltay
    
    return image


################################################################################################3
##
##  Draw the Startup panel
##
################################################################################################3
def drawStartupPanel():
    print("DEBUG: Startup panel")
    #make blank image and get all we need to draw
    image, draw, image_width, image_height = makeBlankPanelImage()

    i = Image.open (os.path.join(PATH_TO_IMG, "logo_weatherstation.png"))

    draw.bitmap ( (0, 0), i )

    ## Drawing finished - display
    #img = image.rotate(90)
    #display on e-Ink
    #eInkShow(epd, img)
    return image

