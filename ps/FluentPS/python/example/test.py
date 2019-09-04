import os
import fps.wrapper as fps
os.environ['DMLC_NUM_SERVER'] = '1'
os.environ['DMLC_NUM_WORKER'] = '10'

os.environ['DMLC_PS_ROOT_URI'] = '127.0.0.1'
os.environ['DMLC_PS_ROOT_PORT'] = '8000'

os.environ['DMLC_NODE_HOST'] = '127.0.0.1'
os.environ['DMLC_NODE_VHOST'] = '127.0.0.1'

os.environ['DMLC_ROLE'] = 'scheduler'

c = fps.scheduler()
c.Init(0)
c.Finalize(0)

