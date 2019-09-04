#!/bin/bash
killall test_ssp
killall caffe
# set -x
if [ $# -lt 3 ]; then
    echo "usage: $0 num_servers num_workers bin [args..]"
    exit -1;
fi

# $1: the number of servers
export DMLC_NUM_SERVER=$1
shift
# $2: the number of workers
export DMLC_NUM_WORKER=$1
shift
# the test program
bin=$1
shift
worker=$1
shift
prototxt=$1
shift
arg="$@"

# start the scheduler
export DMLC_PS_ROOT_URI='10.1.2.64'
export DMLC_PS_ROOT_PORT=8000
export DMLC_ROLE='scheduler'
${bin} ${arg} &

nodes=(se049 se050 se051 se052 se053 se054 se055 se056 se057 se058 se059 se060 se061 se062 se063 se064)

# start servers
# one server at this stage
#export DMLC_ROLE='server'
for ((i=0; i<${DMLC_NUM_SERVER}; ++i)); do
#    export HEAPPROFILE=./S${i}
#    ${bin} ${arg} &
    echo ${nodes[(( $i % ${#nodes[@]} ))]}
    ssh ${nodes[(( $i % ${#nodes[@]} ))]} "cd /localfs/xyao/caffe_pslite; examples/model_zoo/remote_server.sh ${DMLC_NUM_SERVER} ${DMLC_NUM_WORKER} ${bin} $i ${arg}" &
done


# start caffe as workers
# multiple workers 
for ((i=0; i<${DMLC_NUM_WORKER}; ++i)); do
    echo ${nodes[(( $i % ${#nodes[@]} ))]}
    ssh ${nodes[(( $i % ${#nodes[@]} ))]} "cd /localfs/xyao/caffe_pslite; examples/model_zoo/remote_worker.sh ${DMLC_NUM_SERVER} ${DMLC_NUM_WORKER} ${worker} $i ${prototxt}" &
#    ssh ${nodes[(( $i % ${#nodes[@]} ))]} "cd /localfs/xyao/caffe_pslite; export DMLC_NUM_SERVER=1; export DMLC_NUM_WORKER=2; export DMLC_PS_ROOT_URI='10.1.2.64'; export DMLC_PS_ROOT_PORT=8000; export DMLC_ROLE='worker'; export HEAPPROFILE=./W${i}; ${worker} train --solver=examples/mnist/lenet_solver.prototxt " &
done
#export DMLC_ROLE='worker'
#for ((i=0; i<${DMLC_NUM_WORKER}; ++i)); do
#    export HEAPPROFILE=./W${i}
#    ${worker} train --solver=examples/mnist/lenet_solver.prototxt &
#done

wait
