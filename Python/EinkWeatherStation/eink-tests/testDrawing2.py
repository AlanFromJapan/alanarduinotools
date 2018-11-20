
import epd2in13
import time
import Image
import ImageDraw
import ImageFont
from weather import Weather


def getWeather():
    weather = Weather()
    lookup = weather.lookup(26237347) #TKO
    condition = lookup.condition()
    return condition


def main():
    epd = epd2in13.EPD()
    epd.init(epd.lut_full_update)

    # Display resolution in epd2in13.py
    #EPD_WIDTH       = 128
    #EPD_HEIGHT      = 250

    # For simplicity, the arguments are explicit numerical coordinates
#    image = Image.new('1', (epd2in13.EPD_WIDTH, epd2in13.EPD_HEIGHT), 255)  # 255: clear the frame
    image = Image.new('1', (epd2in13.EPD_HEIGHT, epd2in13.EPD_WIDTH), 255)  # 255: clear the frame
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

 
    #Screen is horizontal
    #the TOP LEFT (sticker)  angle is 0,0, BOTTOM RIGHT is max 
    font = ImageFont.truetype('FreeMonoBold.ttf', 18)
    image_width, image_height  = image.size

    imgWeather = Image.open ("sunny.png")
    draw.bitmap ( (image_width - 63, 0), imgWeather)

    #draw.rectangle(x1,y1,x2,y2)
    draw.rectangle((16, 16, 16+8, 16+8), fill = 0)
    draw.rectangle((96, 16, 96+16, 16+16), fill = 0)
    #draw.rectangle((16, 16+128, 16+8, 16+128+8), fill = 0)
        
    draw.line ([(0,0),( image_width, image_height )], fill=0)


    draw.text((image_width/2, image_height/2), "[TEST!]", font = font, fill = 0)

    
    img = image.rotate(90)
    #img = image

    epd.set_frame_memory(img, 0, 0)
    epd.display_frame()
    epd.set_frame_memory(img, 0, 0)
    epd.display_frame()



if __name__ == '__main__':
    main()
