# Bubble Bobble
This is a fan remake of the game Bubble Bobble using raylib and written in c++.
Note that I do not *own* any of the assets (sprites and audio) included in this repository.
You can play the game immediatly here: https://itchybeee.itch.io/bubble-bobble

<img width="2104" height="1794" alt="Screenshot from 2026-04-02 22-21-06" src="https://github.com/user-attachments/assets/b324bfe6-dc68-4899-8111-191c58f141e6" />

## Platforms
This game supports Linux, Windows and Web. See [releases](https://github.com/Gitionia/BubbleBobbleRaylib/releases) for the executables.

## Notes
This project includes the first 24 levels of the original game (with maybe more to come). It includes all the original enemies and implements almost all of the core gameplay mechanics of the original (such as bubble jumping and spawning items).

Used libraries are:
- raylib
- entt
- nlohmann json
- spdlog



## Editor
You can easily create your own levels using [Tiled](https://www.mapeditor.org/). First, either clone this repository or download a [release](https://github.com/Gitionia/BubbleBobbleRaylib/releases). Then, open the project file tiled-config.tiled-project in the res/ folder and you can see the level files in the levels/ folder. You can modify existing levels or create new ones by copying the LevelTemplate.json and renaming it to some LevelXYZ.json. 

Notes:
- Make sure to only place tiles on their respective layer (Tiles on "Tiles" layer, Enemies on "Enemies" layer etc.)
- There are custom properties for the Tiles, Enemies and Airflow layers to e.g. mirror the left half of the level to the right half
- To test a specific level you can run the executable with the following options: 
```console
BubbleBobbleRaylib --levelNr <theLevelNr>                  # or
BubbleBobbleRaylib --levelFile <path/to/LevelXYZ.json>     # the path is optional, it always looks in res/levels/LevelXYZ.json
```
- The debug version has shortcuts to navigate between levels:
  - n: next level
  - m: previous level
  - q: jumps to level 101
  - w: jumps to level 1
  - e: restarts the level
  - some others

- There are custom commands to run the game from within *Tiled*, however they only work for the source code version of this repository
  - This is because the commands expect the executable to be in a build/ folder and a copied res/ folder to be inside the build/ folder
    
<img width="2552" height="1602" alt="image" src="https://github.com/user-attachments/assets/8f358ca0-359b-4086-bd8e-bfd73413874a" />



## Building
### Requirements
- You need cmake and a c++ build system, e.g. MSVC on Windows.

Linux (Ubuntu or similar):
- To install cmake and the compiler, run:
```console
sudo apt install cmake build-essential
```
- To install required c++ headers, run:
```console
sudo apt install libxv-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev
```
- If cmake says, OPENGL_INCLUDE_DIR not found, install libgl1-mesa-dev:
```console
sudo apt install libgl1-mesa-dev
```

Windows:
- Install Visual Studio Build Tools
- Should work out of the box, otherwise some dependecies may need to be resolved

### Compile
```console
mkdir build            # creates the build folder
cmake -S . -B build    # generates platform specific build files (from a "S"ource directory to a "B"uild directory)
cmake --build build    # builds the project into build/ folder
```

### Different configurations
The top commands build the Debug version of the game. 
If you want to build specifically the Debug (default) or Release configuration use the following commands instead:
```console
cmake -S . -B build -DCMAKE_BUILD_TYPE=<the-configuration>  # replace with Debug or Release
cmake --build build
```


