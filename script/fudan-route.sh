#!/bin/bash

if [[ $# != 1 ]]; then
    echo "Usage: `basename $0` <add|delete>"
    exit 1
fi

gw=10.131.250.1
cmd=$1

route $cmd -net 10.6.0.0/16 $gw
route $cmd -net 10.48.0.0/12 $gw # 0011
route $cmd -net 10.64.0.0/10 $gw # 0100
route $cmd -net 10.128.0.0/9 $gw # 1000
route $cmd -net 192.168.0.0/16 $gw
#route $cmd -net 175.0.0.0/8 $gw
#route $cmd -net 178.0.0.0/8 $gw
route $cmd -net 61.129.42.0/24 $gw
route $cmd -net 202.120.64.0/20 $gw
route $cmd -net 202.120.224.0/19 $gw

