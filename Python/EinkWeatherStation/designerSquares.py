#alternate designer 

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

from designer import getImageFromCondition, makeBlankPanelImage, getWeatherImg

################################################################################################3
##
##  Constants & Globals
##
################################################################################################3


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
font_smaller = ImageFont.truetype(os.path.join(PATH_TO_FONTS, 'DisposableDroidBB.ttf'), 12)
font_xsmall = ImageFont.truetype(os.path.join(PATH_TO_FONTS, 'DisposableDroidBB.ttf'), 9)

################################################################################################3
##
##  Functions
##
################################################################################################3


BOX_WIDTH=96

def drawStringCentered(centerXY, msg, font, image, draw):
    msgw, msgh = draw.textsize(msg, font = font)
    draw.text((centerXY[0] -msgw/2, centerXY[1]), msg, font = font, fill = 0)
    return (msgw, msgh)

#draws a weather box with the weather text, temp and image    
#One box is 96 px wide
def drawWeatherBox (w, topleft, image, draw):
    top = topleft[1]
    centerH = topleft[0] + BOX_WIDTH  /2

    #the date
    drawStringCentered((centerH, top ), w["datetimeobj"].strftime("%a %-d %b"), font_smaller, image, draw)
    top = top + 12

    #Time
    drawStringCentered((centerH, top), w["time"][11:16], font_smaller, image, draw)
    top = top +8 #smaller increment since the image top is usually empty so waste less space

    #draw image
    img = getWeatherImg(w)
    draw.bitmap ((centerH - img.width /2, top) , img)
    top = top + img.height 
    
    #temp
    tempsize = drawStringCentered((centerH, top), str(int(w["temp"])), font_medium, image, draw)
    draw.arc ([(centerH + tempsize[0]/2 +4, top), (centerH + tempsize[0]/2 +4+4, top + 4)], 0, 360, fill= 0)
    top = top + 18
    
    #the weather in text
    drawStringCentered((centerH, top ), w["weather"], font_small, image, draw)
    top = top + 18


    
    
    
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

    padding = 16
    #left box : now
    drawWeatherBox(wNow, (padding,0), image, draw)

    #right box : later
    drawWeatherBox(wLater, (250 - BOX_WIDTH - padding,0), image, draw)

    #center arrow
    imgarrow = Image.open (os.path.join(PATH_TO_IMG,"flecheR4.png"))
    draw.bitmap (((250-32)/2,(128-32)/2), imgarrow )

    #not needed, it's already the time shown over the left box 
    #now = lastWeatherDT
    #drawStringCentered((image_width/2, image_height-9), now.strftime("Last update  %H:%M"), font_xsmall, image, draw)


    return image

