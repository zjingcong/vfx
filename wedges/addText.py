#!/usr/bin/env python
# add text to output image

# convert -pointsize 12 -font /usr/share/fonts/truetype/msttcorefonts/verdana.ttf
# -fill white -draw 'gravity NorthWest text 20,20 "frameid: 001 test:lalala" ' lalala.exr lalalatext.exr

import os

def addText():
    command = "convert -fill white -draw 'gravity SouthWest text  10,10 \"" + label + "\"' " + output + " " + output


if __name__ == '__main__':
    print "lalala"
    addText()
