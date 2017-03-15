#!/usr/bin/env python
# add text to output image
# usage: addText.py [input_dir]
# input_dir: parms log file + frame 0-499 .exr
# generate output images into textOutput folder in the input_dir

import os
import sys
import json
import threading
from threading import Thread

OUTPUT_FOLDER = 'textOutput'
# FONT = 'dejavusnas'
FONT = '/usr/share/fonts/truetype/msttcorefonts/verdana.ttf'


def getInfo(jsonfile_path):
    with open(jsonfile_path, 'r') as jsonfile:
        data = json.load(jsonfile)
    return data


def addText(exrfile, frame_id, single_frame_info, output_dir):
    print "Add text for {}...".format(exrfile)
    exrfilename = exrfile.split('/')[-1]
    # generate output file path
    outputfile = os.path.join(output_dir, exrfilename)
    height = 20
    # add frame id
    command = "convert -pointsize 12 -font {font}" \
              " -fill white -draw \'gravity NorthWest text 20,{h} \"frame_id: {frameid}\" \' " \
              "{input} {output}".format(font=FONT, h=height, frameid=frame_id, input=exrfile, output=outputfile)
    os.system(command)
    # add parms value
    for name, value in single_frame_info.iteritems():
        if name != 'frame_id':
            height += 15
            command = "convert -pointsize 12 -font {font}" \
                      " -fill white -draw \'gravity NorthWest text 20,{h} \"{name}: {value}\" \' " \
                      "{input} {output}".format(font=FONT, h=height, name=name, value=value, input=outputfile, output=outputfile)
            os.system(command)


def main():
    # get input dir
    input_dir = sys.argv[1]
    output_dir = os.path.join(input_dir, OUTPUT_FOLDER)
    # get input info
    exr_list = list()
    jsonfile_path = ''
    for root, dir, files in os.walk(input_dir):
        if root == input_dir:
            for name in files:
                if name.endswith('Parms.json'):
                    jsonfile_path = os.path.join(input_dir, name)
                elif name.endswith('.exr'):
                    exrfile = os.path.join(input_dir, name)
                    exr_list.append(exrfile)
            if OUTPUT_FOLDER not in dir:
                # create output folder
                os.system("mkdir {}".format(output_dir))
    # get frame info
    frame_info = getInfo(jsonfile_path)
    # add text for each .exr file
    for exrfile in exr_list:
        frameid = exrfile.split('/')[-1].split('.')[-2]
        if frameid in frame_info:
            single_frame_info = frame_info.get(frameid)
            # multi-threading
            t = Thread(target=addText, args=(exrfile, frameid, single_frame_info, output_dir))
            t.start()
    incomplete = True
    # complete
    while incomplete:
        if threading.activeCount() <= 1:
            incomplete = False
    print "******Complete******"

if __name__ == '__main__':
    main()
