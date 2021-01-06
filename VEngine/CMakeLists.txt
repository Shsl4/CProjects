cmake_minimum_required(VERSION 3.16)
project(DemoGame C)

set(CMAKE_C_STANDARD 11)
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
include_directories(${PROJECT_SOURCE_DIR})

add_executable(DemoGame main.c DemoGame.c VEngine/Source/camera.c VEngine/Source/input.c VEngine/Source/physics.c VEngine/Source/renderer.c VEngine/Source/scene.c VEngine/Source/timer.c VEngine/Source/vector.c VEngine/Source/VEngine.c VEngine/Source/window.c)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

TARGET_LINK_LIBRARIES(DemoGame SDL2 SDL2_ttf -lm)