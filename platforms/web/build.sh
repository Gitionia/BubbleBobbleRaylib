#!/bin/sh

cd build
cmake ../../../ -DCMAKE_TOOLCHAIN_FILE=/home/ben/Tools/RaylibForWeb/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake -DPLATFORM=Web -O3
make

mv BubbleBobbleRaylib.html index.html
zip BubbleBobble.zip index.html BubbleBobbleRaylib.data BubbleBobbleRaylib.js BubbleBobbleRaylib.wasm res/
