cmake_minimum_required(VERSION 3.4)
project(flakysbrain)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES src/main.cpp src/neuron.cpp src/neuron.h src/brain.cpp src/brain.h src/types.h src/event.cpp src/event.h src/performance_measure.cpp src/performance_measure.h)
add_executable(flakysbrain ${SOURCE_FILES})