#!/bin/bash

cd ..
git submodule init
git submodule update

# iniparser compile
cd iniparser
make
cd ..

for name in "libtp3" "libtp12" "libcflora" "node" "gcg" "gos"
do
    cd ${name}
    mkdir release
    cd release
    cmake ..
    make
    cd ../..
done

