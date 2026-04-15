#!/bin/sh

cmake -B build-release -S ../.. -DCMAKE_BUILD_TYPE=Release

cmake --build build-release -j8