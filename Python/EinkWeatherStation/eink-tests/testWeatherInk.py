
import epd2in13
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
    forecasts = lookup.forecast()
    #0 is today, 1 is tomorrow, etc...
    f = forecasts[forecastIndex]
    return  f.text() + ": hight " + str(f2c(f.high())) + " / low " +  str(f2c(f.low()))

def getWeatherToday():
    return "Today is " +getWeather (0)

def getWeatherTomorrow():
    return "Tomorrow is " +getWeather (1)


def main():
    epd = epd2in13.EPD()
    epd.init(epd.lut_full_update)

    # For simplicity, the arguments are explicit numerical coordinates
    image = Image.new('1', (epd2in13.EPD_WIDTH, epd2in13.EPD_HEIGHT), 255)  # 255: clear the frame
    draw = ImageDraw.Draw(image)
    epd.clear_frame_memory(0xFF)
    epd.set_frame_memory(image, 0, 0)
    epd.display_frame()

#    epd.delay_ms(2000)

    # for partial update
    epd.init(epd.lut_partial_update)

##
 # there are 2 memory areas embedded in the e-paper display
 # and once the display is refreshed, the memory area will be auto-toggled,
 # i.e. the next action of SetFrameMemory will set the other memory area
 # therefore you have to set the frame memory twice.
 ##     


    font = ImageFont.truetype('FreeMonoBold.ttf', 16)
    image_width, image_height  = image.size


    # draw a rectangle to clear the image
    draw.rectangle((0, 0, image_width, image_height), fill = 255)
    
    draw.text((0, 0), getWeatherToday(), font = font, fill = 0)
    draw.text((0, 32), getWeatherTomorrow(), font = font, fill = 0)
    draw.text((0, 64), time.strftime('%M:%S'), font = font, fill = 0)
    
    img = image.rotate(90, expand=True)
    #img = image
    image2 = Image.new('1', (epd2in13.EPD_HEIGHT, epd2in13.EPD_WIDTH), 255)  # 255: clear the frame
    draw2 = ImageDraw.Draw(image2)

    draw2.bitmap((0, image_width), img)
        
    epd.set_frame_memory(image2, 0, 0)
    epd.display_frame()
#    epd.set_frame_memory(image2, 0, 0)
#    epd.display_frame()

        
if __name__ == '__main__':
    main()
