
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


    font = ImageFont.truetype('FreeMonoBold.ttf', 18)
    image_width, image_height  = image.size
    ballx=16
    bally= 16
    dx = 1
    dy = 1
    while (True):
        # draw a rectangle to clear the image
#        draw.rectangle((0, 0, image_width, image_height), fill = 255)

#        cond = getWeather()
#        msg = cond.text() + " " +  str(int((float(cond.temp()) - 32.0) / 1.8)) + "C"

        msg = "Bouncing ball"
        
        draw.text((0, 0), msg, font = font, fill = 0)

        draw.point ((ballx, bally), fill = 0)

        ballx = ballx + dx
        bally = bally + dy
        
#        epd.set_frame_memory(image.rotate(270), 0, 0)
        img = image.rotate(90)

        epd.set_frame_memory(img, 0, 0)
        epd.display_frame()
        epd.set_frame_memory(img, 0, 0)
        epd.display_frame()

        time.sleep(1)

if __name__ == '__main__':
    main()
