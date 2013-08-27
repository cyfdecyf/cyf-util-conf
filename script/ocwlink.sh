#!/bin/bash

if [[ $# != 1 ]]; then
    echo "Usage: $0 <url>"
    exit 1
fi

url=$1

head=http://ocw.mit.edu/ans7870
name=`echo $url | sed -e 's/^.*7870//'`
path=$head$name

echo $path
#wget -c $path
