cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
project(NetworkMonitoring)

find_library(TINS_LIBRARY tins)
find_library(SPDLOG spdlog)
add_executable(NetworkMonitoring
src/cmdExecutor.cpp
src/main.cpp
src/networkActivityMonitor)
set(CMAKE_CXX_FLAGS "-Wall -ggdb")
set(CMAKE_CXX_FLAGS_DEBUG "-Wall -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "-Wall -ggdb")
target_include_directories(NetworkMonitoring PRIVATE
    src
    inc)
    target_link_libraries(
    NetworkMonitoring 
    "${SPDLOG}"
    "${TINS_LIBRARY}")