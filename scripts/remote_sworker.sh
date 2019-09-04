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
solver=$1
shift
snap=$1
shift
stype=$1
shift
stale=$1
shift
propa=$1
if [ ! $propa ]; then
propa=1
fi
shift
delaymean=$1
if [ ! $delaymean ]; then
delaymean=0
fi
shift
delaysdev=$1
if [ ! $delaysdev ]; then
delaysdev=0
fi
shift
delaysnode=$1
if [ ! $delaysnode ]; then
delaysnode=1
fi

script_path=`readlink -f $0`
scripts_path=`dirname $script_path`
project_path=`dirname $scripts_path`

scheduler_file=${scripts_path}"/machinefile/scheduler"
host_file=${scripts_path}"/machinefile/host"
v_host_file=${scripts_path}"/machinefile/vhost"

scheduler_ip=`cat $scheduler_file | awk '{ print $0 }'`
host_ip=`cat $host_file | awk '{ print $0 }'`
v_host_ip=`cat $v_host_file | awk '{ print $0 }'`
	
# mark the scheduler, using public IP
declare -a nodes
i=0
for ip in $scheduler_ip; do
	nodes[${i}]=$ip
	(( i++ ))
done
export DMLC_PS_ROOT_URI=${nodes[0]}
export DMLC_PS_ROOT_PORT=8000

# mark the worker, using public IP
i=0
for ip in $host_ip; do
	nodes[${i}]=$ip
	(( i++ ))
done
export DMLC_NODE_HOST=${nodes[(( $no % ${#nodes[@]} ))]}

# mark the worker, using private IP
i=0
for ip in $v_host_ip; do
	nodes[${i}]=$ip
	(( i++ ))
done
export DMLC_NODE_VHOST=${nodes[(( $no % ${#nodes[@]} ))]}

# start caffe as workers
export DMLC_ROLE='worker'
export HEAPPROFILE=./W${no}
# examples/mnist/lenet_solver.prototxt
${worker} train --solver=${solver} --snapshot=${snap} --delay_mean=${delaymean} --delay_sdev=${delaysdev} --delay_node=${delaysnode} 2> ${project_path}/logs/W${no}_caffe_${stype}_${stale}_${propa}_${DMLC_NUM_SERVER}_${DMLC_NUM_WORKER}_${delaymean}_${delaysdev}_${delaysnode} > ${project_path}/logs/W${no}_caffe_${stype}_${stale}_${propa}_${DMLC_NUM_SERVER}_${DMLC_NUM_WORKER}_${delaymean}_${delaysdev}_${delaysnode}_para &

