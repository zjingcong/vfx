#!/usr/bin/env python
# submit ear render task to DPA queue

import os
import time
import datetime

# directory
QUEUE = "velveeta"
SCRIPT_ROOT_DIR = "/DPA/jedi/zjingcong/script/"
PRJ_DIR = "/DPA/jedi/zjingcong/advect"
OUTPUT_ROOT_DIR = "/DPA/jedi/zjingcong/output"
FOLDER_HEAD = 'ears'


start_end_list = [(0, 30), (31, 60), (61, 90), (91, 119)]

# ../config /DPA/jedi/zjingcong/grids/advect_grids_1 ../output 0 0
command = "{exe} {cfg} {grid} {out} {start} {end}"
exe = os.path.join(PRJ_DIR, 'cmake-build-default/ears')
cfg = os.path.join(PRJ_DIR, 'config')
grid = '/DPA/jedi/zjingcong/grids/ear_grids'


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
    for (start_id, end_id) in start_end_list:
        filename = "{}-{:04d}-{:04d}.sh".format(FOLDER_HEAD, start_id, end_id)
        filepath = os.path.join(script_dir, filename)
        f = open(filepath, 'w')
        f.write("#!/bin/bash\n")
        cmd = command.format(exe=exe, start=start_id, end=end_id, out=output_dir,
                             cfg=cfg, grid=grid)
        f.write(cmd)
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
                print "Submit task script {}...".format(script_path)
                os.system("cqsubmittask {queue} {script}".format(queue=QUEUE, script=script_path))
                num += 1

    print "Submission complete."
    print "\t | Task Num: ", num

if __name__ == '__main__':
    submitTask()
