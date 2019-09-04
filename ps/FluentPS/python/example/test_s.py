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

os.environ['DMLC_ROLE'] = 'server'
os.environ['HEAPPROFILE'] = './S' + no

s = fps.server()
s.Init(0)
s.create(0)
s.start(3)
s.Finalize(0)
