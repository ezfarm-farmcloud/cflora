#!/bin/bash

./gos -c ../conf/cflora-server.ini -d >> /dev/null 2>&1
./gcg -c ../conf/cflora-server.ini -v 2 -d  >> /dev/null 2>&1
./node -d  >> /dev/null 2>&1
