#!/bin/bash

cd ..

for name in "libtp3" "libtp12" "libcflora" "node" "gcg" "gos"
do
    cd ${name}
    rm -rf release
    rm -rf debug 
    cd ..
done

