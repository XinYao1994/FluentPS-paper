''' 
This is a local test demo.
To write a distrbuted job, you need to write three python scripts and one shell script.
You can reference to scripts dir: ddl/scripts
'''

''' import env setting '''
import os
import sys
''' import fps package '''
import fps.wrapper as fps

from multiprocessing import Process
import numpy as np
''' rewrite run.sh in it, using os.environ '''

num_servers = 1
num_workers = 2

if len(sys.argv) > 1:
    num_servers = int(sys.argv[1])
    
if len(sys.argv) > 2:
    num_servers = int(sys.argv[2])

os.environ['DMLC_NUM_SERVER'] = str(num_servers)
os.environ['DMLC_NUM_WORKER'] = str(num_workers)

os.environ['DMLC_PS_ROOT_URI'] = '127.0.0.1'
os.environ['DMLC_PS_ROOT_PORT'] = '8000'

os.environ['DMLC_NODE_HOST'] = '127.0.0.1'
os.environ['DMLC_NODE_VHOST'] = '127.0.0.1'


def install_scheduler(name):
    print('Run scheduler task %s (%s)...' % (name, os.getpid()))
    os.environ['DMLC_ROLE'] = 'scheduler'
    c = fps.scheduler()
    c.Init(0)
    c.Finalize(0)

    
def install_server(name):
    print('Run server task %s (%s)...' % (name, os.getpid()))
    os.environ['DMLC_ROLE'] = 'server'
    os.environ['HEAPPROFILE'] = './S' + str(name)
    s = fps.server()
    s.Init(0)
    s.create(0)
    s.start()
    s.Finalize(0)

    
def install_worker(name):
    print('Run worker task %s (%s)...' % (name, os.getpid()))    
    os.environ['DMLC_ROLE'] = 'worker'
    os.environ['HEAPPROFILE'] = './W' + str(name)
    w = fps.worker()
    w.Init(0)
    w.create(0, 0)
    # w.start() we do not need to set slicer
    '''
    do somethings like push/pull
    '''
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
        
    last = 0
    for i in range(repeat):
        print('The worker %s enter iteration %s' % (name, i))
        last = w.spush(keys, vals, i, lens)
        w.wait(w.spull(keys, i))
    w.wait(last)
    w.wait(w.spull(keys, repeat - 1))
    ret = w.read()
    print(ret)
    w.Finalize(0)
    
# num_processes = 1 + num_servers + num_workers

# _pool = Pool(processes=num_processes)

# ret_scheduler = []
# ret_server = []
# ret_worker = []


p_arr = []

for i in range(1):
    p_arr.append(Process(target=install_scheduler, args=(i,)))
    # ret_scheduler.append(_pool.apply_async(install_scheduler, args=(i,)))

for i in range(num_servers):
    p_arr.append(Process(target=install_server, args=(i,)))
    # ret_server.append(_pool.apply_async(install_server, args=(i,)))

for i in range(num_workers):
    p_arr.append(Process(target=install_worker, args=(i,)))
    # ret_worker.append(_pool.apply_async(install_worker, args=(i,)))

for p in p_arr:
    p.start()
    
print('Waiting for all subprocesses done...')

for p in p_arr:
    p.join()
# _pool.close()
# _pool.join()
print('Test SSP is done.')
    
# for result in (ret_scheduler+ret_server+ret_worker):
#     print(result)
