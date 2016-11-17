#!/bin/bash

ps -ef | grep './cflora-mon.sh' | grep -v grep | awk '{print $2}' | xargs kill -9 > /dev/null
sleep 1
ps -ef | grep './node -d' | grep -v grep | awk '{print $2}' | xargs kill > /dev/null
sleep 1
ps -ef | grep './gcg -c' | grep -v grep | awk '{print $2}' | xargs kill > /dev/null
sleep 1
ps -ef | grep './gos -c' | grep -v grep | awk '{print $2}' | xargs kill > /dev/null
