#!/bin/bash

usage() {
    echo "Usage: `basename $0` <path to named.conf> <sqlite3 db> <domain>"
}

if [[ $# != 2 ]]; then
    usage
    exit 1
fi

bindconf=$1
db=$2

if [[ -r $db ]]; then
    # create a backup first
    cp $db $db~
else
    echo "Must use an existing sqlite3 database with schema setup."
fi

sqlite3 $db 'delete from records'
zone2sql --named-conf=$bindconf 2>/dev/null | sqlite3 $db

