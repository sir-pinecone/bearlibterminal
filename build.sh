#!/bin/bash

set -e

mkdir -p Build
cd build
cmake -D CMAKE_CXX_COMPILER="/Library/Developer/CommandLineTools/usr/bin/c++" CMAKE_C_COMPILER="/Library/Developer/CommandLineTools/usr/bin/cc" ..
make
cd ..
