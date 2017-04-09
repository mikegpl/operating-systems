#!/bin/bash

PIDS=`ps aux | awk '{print $2,$11}' | awk '/.\/main/' | cut -d\  -f1`

printf "Horror!\n"
for PID in $PIDS
do
	kill -9 "$PID"
done
