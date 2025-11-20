#!/bin/sh

mkdir build
cd build
emcmake cmake -S ../../../ -B . -DPLATFORM=Web
make

mv BubbleBobbleRaylib.html index.html
zip BubbleBobble.zip index.html BubbleBobbleRaylib.data BubbleBobbleRaylib.js BubbleBobbleRaylib.wasm res/
