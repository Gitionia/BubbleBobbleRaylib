#!/bin/sh


./build.sh
cd build
python3 -m http.server 8080
