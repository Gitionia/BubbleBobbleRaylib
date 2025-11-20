#!/bin/sh

mkdir build
cd build
cmake ../../../ -DCMAKE_TOOLCHAIN_FILE=~/Tools/RaylibForWeb/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake -DPLATFORM=Web
make

mv BubbleBobbleRaylib.html index.html
zip BubbleBobble.zip index.html BubbleBobbleRaylib.data BubbleBobbleRaylib.js BubbleBobbleRaylib.wasm res/
