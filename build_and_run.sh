#!/bin/bash
cd build_debug
cmake ..
cmake --build ./ -- -j4
cd ..
./build_debug/bin/smart_location "$@"
