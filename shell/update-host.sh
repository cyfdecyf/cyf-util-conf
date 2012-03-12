#!/bin/bash

#URL='https://smarthosts.googlecode.com/svn/trunk/hosts'
URL='http://hostsx.googlecode.com/svn/trunk/HostsX.orzhosts'
HOSTS='./hosts'

cd "$( dirname "${BASH_SOURCE[0]}" )"

curl -s -o $HOSTS.new $URL

if [ $? ]; then
    sed -e 's/\r$//' -e '/localhost/d' $HOSTS.new > $HOSTS
else
    echo "download failed"
fi

