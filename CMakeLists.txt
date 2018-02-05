cmake_minimum_required(VERSION 3.4)
project(flakysbrain)

set(CMAKE_CXX_STANDARD 17)
if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
endif (MSVC)

add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE test/catch)

set(SOURCE_FILES src/neuron.cpp src/neuron.h src/brain.cpp src/brain.h src/types.h src/event.cpp src/event.h src/performance_measure.cpp src/performance_measure.h)
add_executable(flakysbrain src/main.cpp ${SOURCE_FILES})

add_executable(flakysbrain_test test/main_test.cpp ${SOURCE_FILES} test/brain_test.cpp)
target_link_libraries(flakysbrain_test Catch)