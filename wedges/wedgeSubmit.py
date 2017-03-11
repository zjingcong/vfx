#!/usr/bin/env python

import os

queue = "brie"
task_num = 10
start_frame = 0
script_dir = "/DPA/wookie/zjingcong/script/"
exe_dir = "/DPA/wookie/zjingcong/wedges/cmake-build-default/wedges"
output_dir = "/DPA/wookie/zjingcong/output"


def submitTask():
    # generate script
    # wedge_type_dict = {"pyro": "-p", "noise": "-n", "wisp": "-w"}
    wedge_type_dict = {"pyro": "-p"}
    for (wedge_type, tag) in wedge_type_dict.items():
        for frame_id in xrange(start_frame, task_num):
            filename = "{}-{:04d}.sh".format(wedge_type, frame_id)
            filepath = os.path.join(script_dir, filename)
            f = open(filepath, 'w')
            f.write("#!/bin/bash\n")
            f.write("{exe} {tag} {frameid} {out}\n".format(exe=exe_dir, tag=tag, frameid=frame_id, out=output_dir))
            f.close()
            # chmod for script file
            os.system("chmod 777 {file}".format(file=filepath))
    print "Scripts generation complete."

    # submit task for each script
    for root, dir, files in os.walk(script_dir):
        for name in files:
            script_path = os.path.join(root, name)
            if script_path.endswith('.sh'):
                frameid = int(script_path.split('-')[1].split('.')[0])
                os.system("cqsubmittask {queue} {script}".format(queue=queue, script=script_path))
                print "Submit task for frame {:04d}...".format(frameid)

    print "Submission complete."
    print "\t | Task Num: ", task_num

if __name__ == '__main__':
    submitTask()
