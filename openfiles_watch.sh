#!/bin/bash

LIST=`pidof nginx`
for ITEM in $LIST ; do
	lsof -p $ITEM | wc -l
done

