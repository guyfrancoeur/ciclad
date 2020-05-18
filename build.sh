#!/bin/bash
#test on Debian for ciclad_v3
apt-get install -y gnuplot
cd ciclad_v3
mkdir -p build
cd build
cmake -G "Unix Makefiles" -DCMAKE_INSTALL_PREFIX=../../../dist ..
cmake --build . --config Release --target all
cmake --build . --config Release --target test
#make -j4
