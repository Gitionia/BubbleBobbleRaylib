cmake_minimum_required(VERSION 3.19)

project(BubbleBobbleRaylib C CXX)

set(CMAKE_CXX_STANDARD 20)

# generates compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_FLAGS_INIT "-fsanitize=undefined")
set(CMAKE_CXX_FLAGS_INIT "-fsanitize=undefined")

message(STATUS "Show Information:")
message(STATUS "Using Generator: ${CMAKE_GENERATOR}")
message(STATUS "Build type: " ${CMAKE_BUILD_TYPE})
message(STATUS "Library Type: " ${LIB_TYPE})
message(STATUS "Compiler flags:" ${CMAKE_CXX_COMPILE_FLAGS})
message(STATUS "Compiler cxx debug flags:" ${CMAKE_CXX_FLAGS_DEBUG})
message(STATUS "Compiler cxx release flags:" ${CMAKE_CXX_FLAGS_RELEASE})
message(STATUS "Compiler cxx min size flags:" ${CMAKE_CXX_FLAGS_MINSIZEREL})
message(STATUS "Compiler cxx flags:" ${CMAKE_CXX_FLAGS})


if(EMSCRIPTEN)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s GL_ENABLE_GET_PROC_ADDRESS=1 -s EXPORTED_RUNTIME_METHODS=HEAPF32 --preload-file res -s FORCE_FILESYSTEM=1")
        set(CMAKE_EXECUTABLE_SUFFIX ".html") # This line is used to set your executable to build with the emscripten html template so that you can directly open it.
endif()

# find src -type f
add_executable(BubbleBobbleRaylib
        src/main.cpp
        src/pch.h
        src/pch.cpp
        src/app/EventSystem.cpp
        src/app/Application.h
        src/app/EventSystem.h
        src/app/Config.h
        src/app/WindowConfig.h
        src/app/Application.cpp
        src/app/StateMachine.h
        src/app/StateMachine.cpp
        src/app/StateMachineGameStates.h
        src/app/StateMachineGameStates.cpp
        src/ecs/EntityFactory.h
        src/ecs/SystemBase.h
        src/ecs/ComponentUtils.h
        src/ecs/EntityFactory.cpp
        src/ecs/Systems.h
        src/ecs/Systems.cpp
        src/ecs/SystemBase.cpp
        src/ecs/Components.h
        src/behavior/GameplayUISystem.h
        src/behavior/GameplayUISystem.cpp
        src/behavior/GameplayWatcherSystem.h
        src/behavior/GameplayWatcherSystem.cpp
        src/behavior/LevelInstantiatorSystem.h
        src/behavior/LevelInstantiatorSystem.cpp
        src/behavior/EnemyTumbleBehaviorSystem.h
        src/behavior/EnemyTumbleBehaviorSystem.cpp
        src/behavior/PositionAnimationBehaviorSystem.cpp
        src/behavior/DragonBehaviorSystem.cpp
        src/behavior/ItemPickupBehaviorSystem.cpp
        src/behavior/DragonBehaviorSystem.h
        src/behavior/WalkingActorUtils.h
        src/behavior/BubbleFloatBehaviorSystem.cpp
        src/behavior/BubblePopBehaviorSystem.cpp
        src/behavior/DragonHitBehaviorSystem.h
        src/behavior/BubbleShootBehaviorSystem.h
        src/behavior/BubbleShootBehaviorSystem.cpp
        src/behavior/BubbleFloatBehaviorSystem.h
        src/behavior/DragonHitBehaviorSystem.cpp
        src/behavior/BubblePopBehaviorSystem.h
        src/behavior/PopEnemyBubbleSystem.cpp
        src/behavior/WalkingActorBehaviorSystem.cpp
        src/behavior/FlyingEnemyBehaviorSystem.cpp
        src/behavior/FlyingEnemyBehaviorSystem.h
        src/behavior/PopEnemyBubbleSystem.h
        src/behavior/WalkingEnemyBehaviorSystem.cpp
        src/behavior/PositionAnimationBehaviorSystem.h
        src/behavior/WalkingActorBehaviorSystem.h
        src/behavior/WalkingEnemyBehaviorSystem.h
        src/behavior/ItemPickupBehaviorSystem.h
        src/behavior/WalkingActorUtils.cpp
        src/behavior/TitleScreenSystem.h
        src/behavior/TitleScreenSystem.cpp
        src/graphics/Sprites.cpp
        src/graphics/RendererSystem.h
        src/graphics/Window.cpp
        src/graphics/RendererSystem.cpp
        src/graphics/Animations.cpp
        src/graphics/Sprites.h
        src/graphics/Animations.h
        src/graphics/Window.h
        src/utils/Log.h
        src/audio/Audio.cpp
        src/audio/Audio.h
        src/level/Enemies.cpp
        src/level/Enemies.h
        src/level/Level.cpp
        src/level/Level.h
        src/level/Physics.h
        src/level/Physics.cpp
        src/level/Items.h
        src/level/Items.cpp
        src/utils/Debug.h
        src/utils/Debug.cpp
        src/utils/Input.cpp
        src/utils/Random.cpp
        src/utils/Utilities.cpp
        src/utils/AnimatedObjects.h
        src/utils/Input.h
        src/utils/Random.h
        src/utils/Utilities.h

)

target_precompile_headers(BubbleBobbleRaylib PUBLIC src/pch.h)


target_compile_definitions(BubbleBobbleRaylib PRIVATE
        $<$<CONFIG:Debug>:DEBUG_TOOLS=1>
)

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
