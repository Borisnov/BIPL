# cmake_minimum_required(VERSION <specify CMake version here>)
project(hardcode)

set(CMAKE_CXX_STANDARD 14)
set(SOURCE_LIB state_machine.h)
set(SOURCE_LIB compiler.h)
set(SOURCE_EXE start.cpp)


add_executable(program ${SOURCE_EXE})
