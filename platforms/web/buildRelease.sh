#!/bin/sh

mkdir build
cd build
emcmake cmake -S ../../../ -B . -DPLATFORM=Web -DCMAKE_BUILD_TYPE=Release
make

mv BubbleBobbleRaylib.html index.html
zip BubbleBobble.zip index.html BubbleBobbleRaylib.data BubbleBobbleRaylib.js BubbleBobbleRaylib.wasm res/
