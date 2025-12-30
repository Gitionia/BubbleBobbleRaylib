cmake_minimum_required(VERSION 3.28)
project(BubbleBobbleRaylib C CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_C_FLAGS_INIT "-fsanitize=undefined")
set(CMAKE_CXX_FLAGS_INIT "-fsanitize=undefined")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

if (EMSCRIPTEN)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s GL_ENABLE_GET_PROC_ADDRESS=1 -s EXPORTED_RUNTIME_METHODS=HEAPF32 --preload-file res -s FORCE_FILESYSTEM=1")
    set(CMAKE_EXECUTABLE_SUFFIX ".html") # This line is used to set your executable to build with the emscripten html template so that you can directly open it.
endif ()

include(FetchContent)


add_compile_definitions(_DEBUG)


add_subdirectory(src)
add_executable(BubbleBobbleRaylib src/app/main.cpp
        src/app/Application.cpp
        src/app/Application.h
        src/graphics/RendererSystem.cpp
        src/graphics/Window.cpp
        src/graphics/Sprites.h
        src/graphics/Sprites.cpp
        src/behavior/Level.h
        src/behavior/Level.cpp
        src/utils/Utilities.h
        src/utils/Utilities.cpp
        src/behavior/DragonBehaviorSystem.cpp
        src/behavior/DragonBehaviorSystem.h
        src/behavior/BubbleBehaviorSystem.cpp
        src/behavior/BubbleBehaviorSystem.h
        src/behavior/Physics.h
        src/behavior/Physics.cpp
        src/ecs/Components.h
        src/ecs/ComponentUtils.h
        src/ecs/EntityFactory.h
        src/ecs/EntityFactory.cpp
        src/ecs/SystemBase.cpp
        src/ecs/SystemBase.h
        src/ecs/Systems.h
        src/ecs/Systems.cpp
        src/app/Config.h
        src/audio/Audio.h
        src/audio/Audio.cpp
        src/graphics/Animations.cpp
        src/graphics/Animations.h
        src/utils/Debug.h
        src/utils/Debug.cpp
        src/utils/Log.h
        src/utils/Input.h
        src/utils/Input.cpp
)

target_compile_options(BubbleBobbleRaylib PUBLIC -fsanitize=undefined)
target_link_options(BubbleBobbleRaylib PUBLIC -fsanitize=undefined)


add_subdirectory("deps/entt-master")
target_link_libraries(BubbleBobbleRaylib PRIVATE EnTT::EnTT)

add_subdirectory("deps/raylib-5.5")
target_link_libraries(BubbleBobbleRaylib PRIVATE raylib)

add_subdirectory("deps/json-3.12")
target_link_libraries(BubbleBobbleRaylib PRIVATE nlohmann_json::nlohmann_json)

add_subdirectory("deps/spdlog-1.16")
target_link_libraries(BubbleBobbleRaylib PRIVATE spdlog::spdlog)


add_custom_target(Copy_Resources
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/res"
        $<TARGET_FILE_DIR:BubbleBobbleRaylib>/res
)
add_dependencies(BubbleBobbleRaylib Copy_Resources)


file(COPY res/sprites/MainSpriteSheet.json DESTINATION ${CMAKE_BINARY_DIR}/res/sprites)
file(COPY res/sprites/MainSpriteSheet.png DESTINATION ${CMAKE_BINARY_DIR}/res/sprites)
file(COPY res/levels/Level1.json DESTINATION ${CMAKE_BINARY_DIR}/res/levels)
