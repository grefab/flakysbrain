cmake_minimum_required(VERSION 3.4)
project(flakysbrain)

set(CMAKE_CXX_STANDARD 17)
if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
endif (MSVC)

add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE test/catch)

set(SOURCE_FILES brain/neuron.cpp brain/neuron.h brain/brain.cpp brain/brain.h brain/types.h brain/event.cpp brain/event.h brain/performance_measure.cpp brain/performance_measure.h)
add_executable(flakysbrain main.cpp ${SOURCE_FILES})

add_executable(flakysbrain_test test/main_test.cpp ${SOURCE_FILES} brain/brain_test.cpp)
target_link_libraries(flakysbrain_test Catch)
