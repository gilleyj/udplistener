#!/bin/bash

gcc udplistener.c -o udplistener

./udplistener 23230 &
./udplistener 23231 &
./udplistener 23232 &

ab -n 1000000 -c 1000 localhost/error > test.txt &

