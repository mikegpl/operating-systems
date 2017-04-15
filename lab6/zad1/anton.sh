#!/bin/bash
QIDS=`ipcs -q | tail -n+4 | awk '{print $2}'`
printf "No country for old queues\n"
for QID in $QIDS
do
	ipcrm -q "$QID"
done
