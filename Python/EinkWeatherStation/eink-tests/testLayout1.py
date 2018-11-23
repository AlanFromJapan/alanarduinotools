

import time
import Image
import ImageDraw
import ImageFont
from weather import Weather


def f2c (temp):
    return (float(temp) - 32.0) / 1.8

def getWeather(forecastIndex):
    weather = Weather()
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

    imgWeather = Image.open ("../retinaweather/retina-snow.png")
    draw.bitmap ( (0, 0), imgWeather)
    imgWeather = Image.open ("../retinaweather/retina-sunny.png")
    draw.bitmap ( (64, 0), imgWeather)
    imgWeather = Image.open ("../retinaweather/retina-rain.png")
    draw.bitmap ( (128, 0), imgWeather)
    imgWeather = Image.open ("../retinaweather/retina-cloud.png")
    draw.bitmap ( (192, 0), imgWeather)

    #draw.rectangle(x1,y1,x2,y2)
    draw.rectangle((16, 16, 16+8, 16+8), fill = 0)
    draw.rectangle((96, 16, 96+16, 16+16), fill = 0)
    #draw.rectangle((16, 16+128, 16+8, 16+128+8), fill = 0)
        
    draw.line ([(0,0),( image_width, image_height )], fill=0)


    text = str(getWeatherToday())
    draw.text((2, image_height/2), text, font = ImageFont.truetype('FreeMonoBold.ttf', 12), fill = 0)
    draw.text((2, image_height/2 + 14), text, font = ImageFont.truetype('ARCADECLASSIC.TTF', 12), fill = 0)
    draw.text((2, image_height/2 + 14*2), text, font = ImageFont.truetype('PixelFJVerdana12pt.ttf', 6), fill = 0)
    draw.text((2, image_height/2 + 14*3), text, font = ImageFont.truetype('DisposableDroidBB.ttf', 16), fill = 0)

    
    img = image.rotate(90)
    #img = image

    image.show()

if __name__ == '__main__':
    main()
