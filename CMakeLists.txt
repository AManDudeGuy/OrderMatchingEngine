cmake_minimum_required(VERSION 3.10)

project(MatchingEngine VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(MatchingEngine source/main.cpp)
add_executable(RandOrderGen source/randOrderGen.cpp)

