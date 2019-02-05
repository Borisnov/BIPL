# cmake_minimum_required(VERSION <specify CMake version here>)
project(hardcode)

set(CMAKE_CXX_STANDARD 14)
set(SOURCE_LIB state_machine.h)
set(SOURCE_EXE syntax.cpp)


add_executable(program ${SOURCE_EXE})
