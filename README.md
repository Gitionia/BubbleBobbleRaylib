# Bubble Bobble
This is a fan remake of the game [Bubble Bobble](https://en.wikipedia.org/wiki/Bubble_Bobble_(video_game)) written in c++ using raylib.
Note that I do not *own* any of the assets (sprites, audio and fonts, see Credit.txt for sources) included in this repository.
You can play the game in the browser here: https://itchybeee.itch.io/bubble-bobble

<img width="2104" height="1794" alt="Screenshot from 2026-04-02 22-21-06" src="https://github.com/user-attachments/assets/b324bfe6-dc68-4899-8111-191c58f141e6" />

## Download
This game supports Linux, Windows and Web. There is a debug and a release, which you can download at [releases](https://github.com/Gitionia/BubbleBobbleRaylib/releases). The debug version has some shortcuts (see Editor section) that are useful, if you want to create your own levels and quickly test them. The release version runs faster and has shortcuts disabled.

## Notes
This project includes the first 33 levels of the original game (with maybe more to come). It includes all the original enemies and implements almost all of the core gameplay mechanics, such as bubble jumping and spawning items, of the original game.

Used libraries are:
- raylib
- entt
- nlohmann json
- spdlog



## Editor
You can create your own levels using [Tiled](https://www.mapeditor.org/). First, either clone this repository or download a [release](https://github.com/Gitionia/BubbleBobbleRaylib/releases). Then, go into the res/ folder and open the project file tiled-config.tiled-project. You can modify the existing levels inside the levels/ folder or create new ones by copying the LevelTemplate.json and renaming it to some LevelXYZ.json. 

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

- There is a custom commands within *Tiled* (File > Commands > Create New Level) that copies the LevelTemplate.json as a new level. 
- There are also commands to run the game normally or start from the current level. However, to use them for the non source code version, you will need to change the paths for the executable to this ./BubbleBobbleRaylib-\<debug or release\>-\<linux or windows\> and the working directory to %projectpath/../
    
<img width="2552" height="1602" alt="image" src="https://github.com/user-attachments/assets/8f358ca0-359b-4086-bd8e-bfd73413874a" />



## Building
### Requirements
- To build the project you need cmake and a c++ compiler.

Linux (Ubuntu or similar):
- To install cmake and the compiler, run:
```console
sudo apt install cmake build-essential
```
- To install required c++ libraries, run:
```console
sudo apt install libxv-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev
```
- If cmake says, OPENGL_INCLUDE_DIR not found, you can install libgl1-mesa-dev:
```console
sudo apt install libgl1-mesa-dev
```

Windows:
- Install Visual Studio Build Tools
- Install cmake
- Should work out of the box, otherwise some dependencies may need to be resolved

### Compile
Run the following to compile the debug version.
```console
mkdir build            # creates the build folder
cmake -S . -B build    # generates platform specific build files (from a "S"ource directory to a "B"uild directory)
cmake --build build    # builds the project into the build/ folder
```

### Different configurations
You can also specify the configuration by running:
```console
cmake -S . -B build -DCMAKE_BUILD_TYPE=<the-configuration>  # replace with Debug or Release
cmake --build build
```


