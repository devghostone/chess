cmake_minimum_required(VERSION 3.20)
project(chess-company)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)

set(SUPPORT_FILEFORMAT_SVG)

add_executable(chess-company src/main.cpp)
add_library(chess-engine src/chess-engine.cpp)
add_library(chess-gui src/gui.cpp)
add_library(chess-multiplayer src/multiplayer.cpp)


add_subdirectory(lib/raylib)
include_directories(src lib/raylib/src)

target_link_libraries(chess-company raylib chess-engine chess-gui chess-multiplayer)
file(COPY resources DESTINATION ${CMAKE_BINARY_DIR})