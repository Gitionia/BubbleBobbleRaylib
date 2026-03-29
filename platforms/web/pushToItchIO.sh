#!/bin/sh

./buildRelease.sh
cd build
butler push BubbleBobble.zip ItchyBeee/bubble-bobble:web-game
