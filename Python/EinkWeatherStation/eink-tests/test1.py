##
 #  @filename   :   main.cpp
 #  @brief      :   2.13inch e-paper display demo
 #  @author     :   Yehui from Waveshare
 #
 #  Copyright (C) Waveshare     September 9 2017
 #
 # Permission is hereby granted, free of charge, to any person obtaining a copy
 # of this software and associated documnetation files (the "Software"), to deal
 # in the Software without restriction, including without limitation the rights
 # to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 # copies of the Software, and to permit persons to  whom the Software is
 # furished to do so, subject to the following conditions:
 #
 # The above copyright notice and this permission notice shall be included in
 # all copies or substantial portions of the Software.
 #
 # THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 # IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 # FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 # AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 # LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 # OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 # THE SOFTWARE.
 ##

import epd2in13
import time
import Image
import ImageDraw
import ImageFont

import socket
import fcntl
import struct


def get_ip_address(ifname):
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        return socket.inet_ntoa(fcntl.ioctl(
            s.fileno(),
            0x8915,  # SIOCGIFADDR
            struct.pack('256s', ifname[:15])
        )[20:24])
    except:
        return "error IP"


def main():
    epd = epd2in13.EPD()
    epd.init(epd.lut_full_update)


    # For simplicity, the arguments are explicit numerical coordinates
    image = Image.new('1', (epd2in13.EPD_WIDTH, epd2in13.EPD_HEIGHT), 255)  # 255: clear the frame
    draw = ImageDraw.Draw(image)
    font = ImageFont.truetype('./FreeMonoBold.ttf', 12)
    draw.rectangle((0, 10, 128, 30), fill = 0)
    draw.text((30, 14), 'Hello Alan!', font = font, fill = 255)
    draw.text((30, 36), 'Hello Nari!', font = font, fill = 0)
    """
    draw.line((16, 60, 56, 60), fill = 0)
    draw.line((56, 60, 56, 110), fill = 0)
    draw.line((16, 110, 56, 110), fill = 0)
    draw.line((16, 110, 16, 60), fill = 0)
    draw.line((16, 60, 56, 110), fill = 0)
    draw.line((56, 60, 16, 110), fill = 0)
    draw.arc((70, 60, 130, 120), 0, 360, fill = 0)
    draw.rectangle((16, 130, 56, 180), fill = 0)
    draw.chord((70, 130, 130, 190), 0, 360, fill = 0)
    """

    draw.text((0, 50), get_ip_address ("wlan0"), font = font, fill = 0)
    
    epd.clear_frame_memory(0xFF)
    epd.set_frame_memory(image, 0, 0)
    epd.display_frame()


if __name__ == '__main__':
    main()
