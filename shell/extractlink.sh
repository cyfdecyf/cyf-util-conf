#!/bin/bash

if [[ $# != 1 ]]; then
    echo "Usage: $0 <URL>"
    exit 1
fi

# curl -s means silent
# grep -o means only print matched text

curl -s $1 | grep -o '<a *href *= *"[^"]*"' | cut -d '"' -s -f 2
