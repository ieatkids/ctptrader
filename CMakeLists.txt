cmake_minimum_required(VERSION 3.20)

project(ctptrader)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-std=c++20 -fPIC -Wall -Werror -Wno-unused-function -Wno-sign-compare -Wno-write-strings -pedantic -pthread")
set(CMAKE_CXX_FLAGS_DEBUG "-g -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -g -DNDEBUG")

# required packages
find_package(Boost REQUIRED COMPONENTS system filesystem)
find_package(GTest REQUIRED)

# pre-compiled packages
set(THIRDPARTY ${CMAKE_SOURCE_DIR}/thirdparty)
include_directories(${THIRDPARTY}/csv-parser/include)
include_directories(${THIRDPARTY}/spdlog/include)
include_directories(${THIRDPARTY}/tomlplusplus/include)
include_directories(${THIRDPARTY}/fmt/include)

include_directories(ctptrader)
add_subdirectory(ctptrader)

enable_testing()
add_subdirectory(test)
