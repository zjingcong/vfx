#!/usr/bin/env python
# submit wand render task to DPA queue

import os
import time
import datetime

# directory
SCRIPT_ROOT_DIR = "/DPA/jedi/zjingcong/script/"
EXE_DIR = "/DPA/jedi/zjingcong/wand/cmake-build-default/wand"
OUTPUT_ROOT_DIR = "/DPA/jedi/zjingcong/output"
FOLDER_HEAD = 'wand'


# frame 0 - 50 (51)
QUEUE = "velveeta"
start_frame = 0
task_num = 51


# frame 51 - 60 (10)
# QUEUE = "velveeta"
# start_frame = 51
# task_num = 61


# frame 61 - 111 (51)
# QUEUE = "velveeta"
# start_frame = 61
# task_num = 112


# frame 112 - 178 (67)
# QUEUE = "velveeta"
# start_frame = 112
# task_num = 179


# frame 179 - 299 (121)
# QUEUE = "velveeta"
# start_frame = 179
# task_num = 300


# frame 300 - 359 (60)
# QUEUE = "velveeta"
# start_frame = 300
# task_num = 360


def submitTask():
    timestamp = time.time()
    dates = datetime.datetime.fromtimestamp(timestamp).strftime('%Y-%m-%d-%H-%M-%S')
    # generate script
    folder_name = "{}-{}".format(FOLDER_HEAD, dates)
    script_dir = os.path.join(SCRIPT_ROOT_DIR, folder_name)
    output_dir = os.path.join(OUTPUT_ROOT_DIR, folder_name)
    print "Script path: ", script_dir
    print "Output path: ", output_dir
    # create script path
    os.system("mkdir {}".format(script_dir))
    os.system("chmod 770 {}".format(script_dir))
    # create output path
    os.system("mkdir {}".format(output_dir))
    os.system("chmod 770 {}".format(output_dir))
    # create scripts
    for frame_id in xrange(start_frame, task_num):
        filename = "{}-{:04d}.sh".format(FOLDER_HEAD, frame_id)
        filepath = os.path.join(script_dir, filename)
        f = open(filepath, 'w')
        f.write("#!/bin/bash\n")
        f.write("{exe} -output {out} -id {frameid}\n".format(exe=EXE_DIR, frameid=frame_id, out=output_dir))
        f.close()
        # chmod for script file
        os.system("chmod 770 {file}".format(file=filepath))
    print "Scripts generation complete."

    # submit task for each script
    num = 0
    script_dir = os.path.join(SCRIPT_ROOT_DIR, folder_name)
    for root, dir, files in os.walk(script_dir):
        for name in files:
            script_path = os.path.join(root, name)
            # there's .log file in the same dir
            if script_path.endswith('.sh'):
                frameid = int(script_path.split('/')[-1].split('-')[1].split('.')[0])
                print "Submit task for frame {:04d} from script {}...".format(frameid, script_path)
                os.system("cqsubmittask {queue} {script}".format(queue=QUEUE, script=script_path))
                num += 1

    print "Submission complete."
    print "\t | Task Num: ", num

if __name__ == '__main__':
    submitTask()
