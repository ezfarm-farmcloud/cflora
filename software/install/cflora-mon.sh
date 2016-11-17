#!/bin/bash

TARGET_DIR="/usr/local/cflora"
while [ 1 ]
   do
		pid_s=`ps -ef | grep "./gos -c" | grep -v 'grep' | awk '{print $2}'`
		pid_g=`ps -ef | grep "./gcg -c" | grep -v 'grep' | awk '{print $2}'`
		pid_n=`ps -ef | grep "./node -d" | grep -v 'grep' | awk '{print $2}'`
		if [ -z $pid_s ]; then
			cd "$TARGET_DIR/bin"
			$TARGET_DIR/bin/cflora-restart.sh
		elif [ -z $pid_g ]; then
			cd "$TARGET_DIR/bin"
			$TARGET_DIR/bin/cflora-restart.sh
		elif [ -z $pid_n ]; then
			cd "$TARGET_DIR/bin"
			$TARGET_DIR/bin/cflora-restart.sh
		fi
		sleep 6
   done
   
