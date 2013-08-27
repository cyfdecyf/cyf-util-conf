#!/bin/bash

cmd=( "usr.tar.gz /usr"
      "opt.tar.gz /opt"
      'other.tar.gz --exclude=/usr --exclude=/opt --exclude=/var/log/* --exclude=/var/tmp/* --exclude=/var/cache/squid --exclude=/srv/* --exclude=/tmp/* --exclude=/mnt/* --exclude=/home/* --exclude=/proc/* --exclude=/sys/* /'
    )

num=`expr ${#cmd[@]} - 1`
for i in `seq 2 $num`; do
        echo "tar czppf ${cmd[$i]}"
        tar czppf ${cmd[$i]}
done

