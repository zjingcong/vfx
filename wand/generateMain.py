
import os

frame_num = 30

cwd = os.getcwd()
exe_path = os.path.join(cwd, 'cmake-build-default', 'wand')
out_path = os.path.join(cwd, 'output', 'boom_test_1')
log_path = os.path.join(cwd, 'log', 'generateWisps.log')

for frame_id in xrange(frame_num):
    command = "{exe} -output {out} -id {id}".format(exe=exe_path, out=out_path, id=frame_id)
    os.system(command)
