#!/bin/sh

cmake -B build-release -S ../..                         \
    -DCMAKE_BUILD_TYPE=Release                          \
    -DCMAKE_SYSTEM_NAME=Windows                         \
    -DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc           \
    -DCMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++         \
    -DCMAKE_EXE_LINKER_FLAGS="-static -static-libgcc -static-libstdc++"

cmake --build build-release
