cmake_minimum_required(VERSION 3.4)
project(flakysbrain)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp neuron.cpp neuron.h brain.cpp brain.h types.h event.cpp event.h performance_measure.cpp performance_measure.h)
add_executable(flakysbrain ${SOURCE_FILES})