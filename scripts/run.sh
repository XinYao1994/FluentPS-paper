#!/bin/bash
# set -x
if [ $# -lt 3 ]; then
    echo "usage: $0 num_servers num_workers bin [args..]"
    exit -1;
fi

export DMLC_NUM_SERVER=$1
shift
# $2: the number of workers
export DMLC_NUM_WORKER=$1
shift

script_path=`readlink -f $0`
scripts_path=`dirname $script_path`
project_path=`dirname $scripts_path`

model=$1
shift
# the test program
bin=${project_path}"/"${model}"/"$1
shift
worker=${project_path}"/"${model}"/"$1
shift
solver=${project_path}"/"${model}"/"$1
shift
arg="$@"

scheduler_file=${scripts_path}"/machinefile/scheduler"
v_scheduler_file=${scripts_path}"/machinefile/vscheduler"
host_file=${scripts_path}"/machinefile/host"
server_file=${scripts_path}"/machinefile/server"

scheduler_ip=`cat $scheduler_file | awk '{ print $0 }'`
v_scheduler_ip=`cat $v_scheduler_file | awk '{ print $0 }'`
host_ip=`cat $host_file | awk '{ print $0 }'`
server_ip=`cat $server_file | awk '{ print $0 }'`

declare -a nodes
# public IP
i=0
for ip in $scheduler_ip; do
	nodes[${i}]=$ip
	(( i++ ))
done
export DMLC_PS_ROOT_URI=${nodes[0]}
export DMLC_PS_ROOT_PORT=8000

# private IP
i=0
for ip in $v_scheduler_ip; do
	nodes[${i}]=$ip
	(( i++ ))
done
export DMLC_NODE_VHOST=${nodes[0]}

# server IP
declare -a servers
i=0
for ip in $server_ip; do
	servers[${i}]=$ip
	(( i++ ))
done
# worker IP
i=0
for ip in $host_ip; do
	nodes[${i}]=$ip
	(( i++ ))
done

# check before running
echo $DMLC_PS_ROOT_URI
echo ${project_path}
echo ${bin}
echo ${worker}
echo ${solver}
echo ${arg}
# exit

# Must be on node scheduler_ip
echo "create scheduler"
export DMLC_ROLE='scheduler'
${bin} ${arg} --sync_timeout=600 &

echo "create servers"
for ((i=0; i<${DMLC_NUM_SERVER}; ++i)); do
    echo ${servers[(( $i % ${#servers[@]} ))]}
    ssh ${servers[(( $i % ${#servers[@]} ))]} "${scripts_path}/remote_server.sh ${DMLC_NUM_SERVER} ${DMLC_NUM_WORKER} ${bin} $i ${arg}" &
done

echo "create workers"
for ((i=0; i<${DMLC_NUM_WORKER}; ++i)); do
    echo ${nodes[(( $i % ${#nodes[@]} ))]}
    ssh ${nodes[(( $i % ${#nodes[@]} ))]} "cd ${project_path}/${model}; ${scripts_path}/remote_worker.sh ${DMLC_NUM_SERVER} ${DMLC_NUM_WORKER} ${worker} $i ${solver} ${arg}" &
done

wait %1
