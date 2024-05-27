cmake_minimum_required(VERSION 3.20)
project(chess-company)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)

set(SUPPORT_FILEFORMAT_SVG)

add_executable(chess-company src/main.cpp)
add_library(chess-engine src/chess-engine.cpp)
add_library(chess-gui src/gui.cpp)
add_library(chess-multiplayer src/multiplayer.cpp)

include_directories(src lib/raylib/src)

if(EMSCRIPTEN)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s ERROR_ON_UNDEFINED_SYMBOLS=0 -sALLOW_MEMORY_GROWTH --bind --memory")
    set(CMAKE_EXECUTABLE_SUFFIX ".html") # This line is used to set your executable to build with the emscripten html template so that you can directly open it.
    include_directories(lib/datachannel-wasm/wasm/include)
    add_subdirectory(lib/datachannel-wasm EXCLUDE_FROM_ALL)
    find_library(RL_LIBRARY NAMES raylib PATHS lib/raylib/src NO_DEFAULT_PATH NO_CMAKE_FIND_ROOT_PATH)
    set_target_properties(chess-company PROPERTIES LINK_FLAGS "--preload-file ${CMAKE_SOURCE_DIR}/resources@resources")
    target_link_libraries(chess-company ${RL_LIBRARY} chess-engine chess-gui chess-multiplayer datachannel-wasm)
else()
    add_subdirectory(lib/raylib)
    add_subdirectory(lib/libdatachannel)
    include_directories(lib/libdatachannel/include)
    target_link_libraries(chess-company raylib chess-engine chess-gui chess-multiplayer datachannel)
    file(COPY resources DESTINATION ${CMAKE_BINARY_DIR})
endif()