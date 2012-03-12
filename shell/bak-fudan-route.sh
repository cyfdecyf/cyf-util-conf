#!/bin/bash

if [[ $# != 1 ]]; then
    echo "Usage: `basename $0` <add|delete>"
    exit 1
fi

gw=10.131.250.1
cmd=$1

route $cmd -net 10.6.0.0/16 $gw
route $cmd -net 10.49.0.0/16 $gw
route $cmd -net 10.64.0.0/16 $gw
route $cmd -net 10.107.0.0/16 $gw
route $cmd -net 10.108.0.0/16 $gw
route $cmd -net 10.109.0.0/16 $gw
route $cmd -net 10.131.0.0/16 $gw
route $cmd -net 10.132.0.0/16 $gw
route $cmd -net 10.171.0.0/16 $gw
route $cmd -net 10.240.0.0/16 $gw
route $cmd -net 192.168.0.0/16 $gw
route $cmd -net 175.0.0.0/8 $gw
route $cmd -net 178.0.0.0/8 $gw
route $cmd -net 61.129.42.0/24 $gw
route $cmd -net 202.120.64.0/20 $gw
route $cmd -net 202.120.224.0/19 $gw

