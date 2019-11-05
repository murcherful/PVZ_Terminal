cmake_minimum_required(VERSION 2.7)

project(Game)

set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

set(CMAKE_EXE_LINKER_FLAGS "-std=c++11")

add_executable(Game main.cpp ScreenDraw.cpp KeyBoard.cpp MainControl.cpp Scene.cpp Object.cpp)
