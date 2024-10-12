cmake_minimum_required(VERSION 3.30)
project(simple)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(LLVM REQUIRED CONFIG)

include_directories(${LLVM_INCLUDE_DIRS})

include_directories(./include)
link_directories(${LLVM_LIBRARY_DIRS})
add_definitions(${LLVM_DEFINITIONS})

file(GLOB SOURCES "./src/*.cpp")
add_executable(simple ${SOURCES})

target_link_libraries(simple LLVM)