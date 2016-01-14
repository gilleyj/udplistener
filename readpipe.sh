#!/bin/bash

pipe=/tmp/testpipe

trap "rm -f $pipe" EXIT

if [ ! -p $pipe ]; then
    mkfifo $pipe
    chown www-data:www-data $pipe
    chmod a+rw $pipe
fi

while true
do
    if read line <$pipe; then
        echo $line
    fi
done

