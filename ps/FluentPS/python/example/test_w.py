import os
import sys
import fps.wrapper as fps

no = '0'
if len(sys.argv) > 1:
    no = sys.argv[1]

os.environ['DMLC_NUM_SERVER'] = '1'
os.environ['DMLC_NUM_WORKER'] = '10'

os.environ['DMLC_PS_ROOT_URI'] = '127.0.0.1'
os.environ['DMLC_PS_ROOT_PORT'] = '8000'

os.environ['DMLC_NODE_HOST'] = '127.0.0.1'
os.environ['DMLC_NODE_VHOST'] = '127.0.0.1'

os.environ['DMLC_ROLE'] = 'worker'
os.environ['HEAPPROFILE'] = './W' + no

w = fps.worker()
w.Init(0)
w.create(0, 0)
repeat = 50
num = 10000
keys = []
vals = []
lens = []
for i in range(10):
    keys.append(i)

for i in range(10):
    lens.append(1000)

for i in range(num):
    vals.append(float(i))

print(len(keys))
print(len(lens))
print(len(vals))
last = 0
for i in range(repeat):
    print('The worker %s enter iteration %s' % (no, i))
    last = w.spush(keys, vals, i, lens)
    w.wait(w.spull(keys, i))
w.wait(last)
w.wait(w.spull(keys, repeat - 1))
# ret = w.read()
# print(ret)
w.Finalize(0)
# w.start()
# w.Finalize(0)
