cmake_minimum_required(VERSION 3.30)
project(terminalGraphicEngine)

set(CMAKE_CXX_STANDARD 20)

add_executable(terminalGraphicEngine main.cpp
        src/terminal/terminal.cpp
        src/terminal/gnomeTerminalWayland.cpp
        src/math/matrix.cpp)

target_include_directories(terminalGraphicEngine PUBLIC ${PROJECT_SOURCE_DIR}/include)
