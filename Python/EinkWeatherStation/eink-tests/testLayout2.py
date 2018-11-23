
import datetime
import time
import Image
import ImageDraw
import ImageFont
from weather import Weather, Unit

PADDING = 10

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
    EPD_WIDTH       = 128
    EPD_HEIGHT      = 250

    # For simplicity, the arguments are explicit numerical coordinates
#    image = Image.new('1', (epd2in13.EPD_WIDTH, epd2in13.EPD_HEIGHT), 255)  # 255: clear the frame
    image = Image.new('1', (EPD_HEIGHT, EPD_WIDTH), 255)  # 255: clear the frame
    draw = ImageDraw.Draw(image)

    #Screen is horizontal
    #the TOP LEFT (sticker)  angle is 0,0, BOTTOM RIGHT is max 
    image_width, image_height  = image.size


    w = ["unknown", 0, 0]
    font24 = ImageFont.truetype('DisposableDroidBB.ttf', 24)
    font16 = ImageFont.truetype('DisposableDroidBB.ttf', 16)
    
    try:
        pass
        #w = getWeather (0) #0 = today, 1 = tomorrow, ...
    except BaseException,ex:
        #something wrong happened
        print (ex)
        pass
        

    try:
        imgWeather = Image.open ("../retinaweather/" + getImageFromCondition(w[0].lower()))
        
        draw.bitmap ( (EPD_HEIGHT - 64 - PADDING, 0), imgWeather)
    except BaseException,ex:
        #failed to find the image most likey
        print (ex)
        pass
    #draw.rectangle(x1,y1,x2,y2)
#    draw.rectangle((16, 16, 16+8, 16+8), fill = 0)
#    draw.rectangle((96, 16, 96+16, 16+16), fill = 0)
    #draw.rectangle((16, 16+128, 16+8, 16+128+8), fill = 0)

    #horizontal line
    draw.line ([(0, 64),( image_width, 64 )], fill=0)

    ###########################################3
    ##
    ## TOP PART : WEATHER
    ##
    #the weather in text
    draw.text((PADDING, PADDING), w[0], font = font24, fill = 0)
    #temp min-max
    draw.text((PADDING, PADDING + 24), "min = %dC, max = %dC" % (w[1], w[2]), font = font16, fill = 0)


    ###########################################3
    ##
    ## BOTTOM PART : Misc info
    ##
    now = datetime.datetime.now()
    draw.text((PADDING, PADDING +60), now.strftime("%A"), font = font24, fill = 0)
    draw.text((PADDING, PADDING +60+24), now.strftime("%Y/%m/%d"), font = font16, fill = 0)
    
    
    img = image.rotate(90)

    image.show()

if __name__ == '__main__':
    main()
