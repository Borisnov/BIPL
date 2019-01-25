# cmake_minimum_required(VERSION <specify CMake version here>)
project(hardcode)

set(CMAKE_CXX_STANDARD 14)
set(SOURCE_LIB state_machine.h)
set(SOURCE_EXE sintacsis.cpp)



#add_library(state_machine STATIC ${SOURCE_LIB})
#add_library(state_machine.h)
add_executable(gramma ${SOURCE_EXE})
add_executable(test tasks.cpp)
add_executable(main main.cpp)
