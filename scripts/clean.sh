#!/bin/bash

# the test program
bin=$1
shift
worker=$1
shift
arg="$@"

script_path=`readlink -f $0`
scripts_path=`dirname $script_path`
project_path=`dirname $scripts_path`

scheduler_file=$scripts_path+"/machinefile/scheduler"
host_file=$scripts_path+"/machinefile/host"

scheduler_ip=`cat $scheduler_file | awk '{ print $2 }'`
host_ip=`cat $host_file | awk '{ print $2 }'`

for ip in $scheduler_ip; do
	ssh $ip "killall ${bin}"
done

for ip in $host_ip; do
	ssh $ip "killall ${bin}"
	ssh $ip "killall ${worker}"
done

echo "All done!"
