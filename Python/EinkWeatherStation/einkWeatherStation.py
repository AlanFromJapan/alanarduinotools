
import epd2in13
import datetime
import time
import Image
import ImageDraw
import ImageFont
from weather import Weather, Unit

PADDING = 5
PATH_TO_FONTS = "fonts/"
PATH_TO_IMG = "retinaweather/"

condition2image = {
    "mosly sunny" : "retina-cloud-part.png",
    "partly cloudy": "retina-cloud-part.png",    

    "sunny" : "retina-sunny.png",
    "fair" : "retina-sunny.png",
    "hot" : "retina-sunny.png",

    "mostly cloudy": "retina-cloud.png",

    "scattered showers" : "retina-rain.png",

    "snow" : "retina-snow.png"
    
}


def moonPhase1(d):
    #http://www.voidware.com/moon_phase.htm
    y = d.year
    m = d.month
    d = d.day

    if m < 3:
        y = y -1
        m = m + 12
    m = m + 1
    c = 365.25 * y
    e = 30.6 * m
    jd  = c + e + d - 694039.09
    jd = jd / 29.53

    jd = jd - float(int (jd))
    b = jd * 8 + 0.5
    b = b % 8
    return b



def getWeather(forecastIndex):
    weather = Weather(unit=Unit.CELSIUS)
    lookup = weather.lookup(26237347) #TKO
    #condition = lookup.condition()
    forecasts = lookup.forecast
    #0 is today, 1 is tomorrow, etc...
    f = forecasts[forecastIndex]
#    return  [f.text, f2c(f.high), f2c(f.low)]
    return  [f.text, float(f.high), float(f.low)]



def getWeatherToday():
    w = getWeather (0)
    return "Today is %s Min %d Max %d"  % (w[0], w[2], w[1]) 



def getWeatherTomorrow():
    return "Tomorrow is " +getWeather (1)[0]



def getImageFromCondition(condition):
    if condition in condition2image: 
        return condition2image[condition]
    else:
        return "unknown.png"

    
    
def main():

    # Display resolution in epd2in13.py
    #EPD_WIDTH       = 128
    #EPD_HEIGHT      = 250

    # For simplicity, the arguments are explicit numerical coordinates
    image = Image.new('1', (epd2in13.EPD_HEIGHT, epd2in13.EPD_WIDTH), 255)  # 255: clear the frame
#    image = Image.new('1', (EPD_HEIGHT, EPD_WIDTH), 255)  # 255: clear the frame
    draw = ImageDraw.Draw(image)

    
    #init the e-Ink
    epd = epd2in13.EPD()
    epd.init(epd.lut_full_update)

    epd.clear_frame_memory(0xFF)
    epd.set_frame_memory(image, 0, 0)
    epd.display_frame()
#    epd.init(epd.lut_partial_update)

    
    #Screen is horizontal
    #the TOP LEFT (sticker)  angle is 0,0, BOTTOM RIGHT is max 
    image_width, image_height  = image.size


    w = ["unknown", 0, 0]
    fong_big = ImageFont.truetype(PATH_TO_FONTS + 'DisposableDroidBB.ttf', 30)
    font_medium = ImageFont.truetype(PATH_TO_FONTS + 'DisposableDroidBB.ttf', 20)
    font_small = ImageFont.truetype(PATH_TO_FONTS + 'DisposableDroidBB.ttf', 16)
    font_xsmall = ImageFont.truetype(PATH_TO_FONTS + 'DisposableDroidBB.ttf', 9)
    
    try:
        pass
        w = getWeather (0) #0 = today, 1 = tomorrow, ...
    except BaseException,ex:
        #something wrong happened
        print (ex)
        pass
        

    try:
        imgWeather = Image.open (PATH_TO_IMG + getImageFromCondition(w[0].lower()))
        
        draw.bitmap ( (epd2in13.EPD_HEIGHT - 64 - PADDING, 0), imgWeather)
    except BaseException,ex:
        #failed to find the image most likey
        print (ex)
        pass

    #horizontal line
    draw.line ([(0, 64),( image_width, 64 )], fill=0)

    ###########################################3
    ##
    ## TOP PART : WEATHER
    ##
    #the weather in text
    draw.text((PADDING, PADDING), w[0], font = fong_big, fill = 0)
    #temp min-max
    draw.text((PADDING, PADDING + 30), "min = %dC, max = %dC" % (w[1], w[2]), font = font_medium, fill = 0)


    ###########################################3
    ##
    ## BOTTOM PART : Misc info
    ##
    now = datetime.datetime.now()
    draw.text((PADDING, PADDING +60), now.strftime("%A"), font = fong_big, fill = 0)
    draw.text((PADDING, PADDING +60+30), now.strftime("%Y/%m/%d"), font = font_small, fill = 0)
    draw.text((PADDING, image_height - 14), now.strftime("Last update  %H:%M"), font = font_xsmall, fill = 0)

    # Moon
    phase = moonPhase1(now)
    basex = image_width - 50
    basey = 70
    moonw = 40
    moonh = 35
    draw.text((basex + 4, basey + moonh + 1), "Moon", font=font_small, fill = 0)
    #ext rectangle
    draw.rectangle ([ (basex, basey) , (basex + moonw, basey + moonh)], outline = 0)
    #in color
    if phase == 0:
        #new moon - all black
        draw.rectangle ([ (basex +2 , basey+2) , (basex + moonw - 2, basey + moonh-2 )], outline = 0, fill = 0)
    elif phase == 1:
        draw.rectangle ([ (basex +2 , basey+2) , (basex + (moonw * 0.75) - 2, basey + moonh-2 )], outline = 0, fill = 0)
    elif phase == 2:
        draw.rectangle ([ (basex +2 , basey+2) , (basex + (moonw * 0.5) - 2, basey + moonh-2 )], outline = 0, fill = 0)
    elif phase == 3:
        draw.rectangle ([ (basex +2 , basey+2) , (basex + (moonw * 0.25) - 2, basey + moonh-2 )], outline = 0, fill = 0)
    elif phase == 4:
        #fullmoon
        pass
    elif phase == 5:
        draw.rectangle ([ (basex +2 + (moonw * 0.75) , basey+2) , (basex + moonw - 2, basey + moonh-2 )], outline = 0, fill = 0)
    elif phase == 6:
        draw.rectangle ([ (basex +2 + (moonw * 0.5) , basey+2) , (basex + moonw - 2, basey + moonh-2 )], outline = 0, fill = 0)
    elif phase == 7:
        draw.rectangle ([ (basex +2 + (moonw * 0.25) , basey+2) , (basex + moonw - 2, basey + moonh-2 )], outline = 0, fill = 0)

        
    ###########################################3
    ##
    ## Drawinf finished - display
    ##
    img = image.rotate(90)

    #display on e-Ink
    epd.set_frame_memory(img, 0, 0)
    epd.display_frame()

    
if __name__ == '__main__':
    main()
