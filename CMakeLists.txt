cmake_minimum_required(VERSION 3.20)
project(chess-company)

add_executable(chess-company src/main.cpp)
add_library(chess-engine src/chess-engine.cpp)
add_library(chess-gui src/gui.cpp)

add_subdirectory(lib/raylib)
include_directories(src lib/raylib/src)

target_link_libraries(chess-company raylib chess-engine chess-gui)