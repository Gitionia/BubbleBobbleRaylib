# Bubble Bobble
This is a fan remake of the game Bubble Bobble using raylib and written in c++.
Note that I do not *own* any of the assets (sprites and audio) included in this repository.
You can play the game immediatly here: https://itchybeee.itch.io/bubble-bobble

<img width="2104" height="1794" alt="Screenshot from 2026-04-02 15-21-36" src="https://github.com/user-attachments/assets/830b458e-e1c7-4dba-9e72-2737e3d9ffd4" />

## Platforms
This game supports Linux, Windows and Web. 

## Features
This project includes the first 24 levels of the original game (with maybe more to come). It includes all the original enemies and implements almost all of the core gameplay mechanics of the original (such as bubble jumping and spawning items).

## Editor
The project uses *Tiled* to create the levels (in res/levels). It should be pretty easy to create your own levels using the level template (res/levels/LevelTemplate.json). Just make sure to place tiles of a tileset only on the corresponding layer (tiles on the tile layer, enemies on the enemy layer, etc.). The game always starts at level 1, however, in the Debug configuration, pressing q while playing switches to level 101 and continues from there. Pressing n and m increments or decrements the level.  

## Building

The Build system being used is cmake. That means you can build it using (if you have cmake and a c++ build system installed):
```console
mkdir build
cmake -S . -B build/
cmake --build build
```


