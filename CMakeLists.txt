cmake_minimum_required(VERSION 3.28)
project(BubbleBobbleRaylib)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_C_FLAGS_INIT "-fsanitize=disabled")
set(CMAKE_CXX_FLAGS_INIT "-fsanitize=disabled")

include(FetchContent)

add_subdirectory(src)
add_executable(BubbleBobbleRaylib src/app/main.cpp
        src/app/Application.cpp
        src/app/Application.h
        src/ecs/Systems.cpp
        src/graphics/RendererSystem.cpp
        src/graphics/Window.cpp
        src/graphics/Sprites.h
        src/graphics/Sprites.cpp
        src/behavior/Level.h
        src/behavior/Level.cpp
        src/Utilities.h
        src/Utilities.cpp
        src/behavior/DragonBehaviorSystem.cpp
        src/behavior/DragonBehaviorSystem.h
        src/behavior/BubbleBehaviorSystem.cpp
        src/behavior/BubbleBehaviorSystem.h
        src/behavior/Physics.h
        src/behavior/Physics.cpp
        src/ecs/SystemBase.cpp
        src/ecs/SystemBase.h
        src/app/Config.h
)

target_compile_options(BubbleBobbleRaylib PUBLIC -fsanitize=undefined)
target_link_options(BubbleBobbleRaylib PUBLIC -fsanitize=undefined)
target_compile_options(BubbleBobbleRaylib PUBLIC -fsanitize=undefined)
target_link_options(BubbleBobbleRaylib PUBLIC -fsanitize=undefined)

# add_subdirectory("deps/glfw-3.4")
# target_link_libraries(BubbleBobbleRaylib PRIVATE glfw)
add_subdirectory("deps/entt-master")
target_link_libraries(BubbleBobbleRaylib PRIVATE EnTT::EnTT)

add_subdirectory("deps/raylib-5.5")
# find_package(raylib REQUIRED)
target_link_libraries(BubbleBobbleRaylib PRIVATE raylib)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
FetchContent_MakeAvailable(json)
# find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(BubbleBobbleRaylib PRIVATE nlohmann_json::nlohmann_json)

add_custom_target(Copy_Resources
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/res"
        $<TARGET_FILE_DIR:BubbleBobbleRaylib>/res
)
add_dependencies(BubbleBobbleRaylib Copy_Resources)

file(COPY res/sprites/MainSpriteSheet.json DESTINATION ${CMAKE_BINARY_DIR}/res/sprites)
file(COPY res/sprites/MainSpriteSheet.png DESTINATION ${CMAKE_BINARY_DIR}/res/sprites)
file(COPY res/levels/Level1.json DESTINATION ${CMAKE_BINARY_DIR}/res/levels)



