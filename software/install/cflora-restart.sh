#!/bin/bash

./cflora-stop.sh > /dev/null
./cflora-run.sh
./cflora-mon.sh & > /dev/null
