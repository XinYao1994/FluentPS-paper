#!/bin/bash
# set -x
if [ $# -lt 3 ]; then
    echo "usage: $0 num_servers num_workers bin [args..]"
    exit -1;
fi

export DMLC_NUM_SERVER=$1
shift
export DMLC_NUM_WORKER=$1
shift
worker=$1
shift
no=$1
shift
prototxt=$1
shift
arg="$@"

# start the scheduler
export DMLC_PS_ROOT_URI='10.1.2.64'
export DMLC_PS_ROOT_PORT=8000


# start caffe as workers
export DMLC_ROLE='worker'
#for ((i=0; i<${DMLC_NUM_WORKER}; ++i)); do
export HEAPPROFILE=./W${no}
${worker} train --solver=${prototxt} &

wait
