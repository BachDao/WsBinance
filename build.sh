#!/bin/bash

cd /build
rm -rf cmake-build
mkdir -p cmake-build

cd ./cmake-build && cmake ..
cmake --build .