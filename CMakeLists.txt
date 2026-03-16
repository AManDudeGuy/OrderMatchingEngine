cmake_minimum_required(VERSION 3.14)

project(MatchingEngine VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)

FetchContent_Declare(
    googleBenchMark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG v1.9.5
    EXCLUDE_FROM_ALL
)


set(BENCHMARK_ENABLE_GTEST_TESTS OFF)
set(BENCHMARK_ENABLE_TESTING OFF)
set(BENCHMARK_INSTALL_DOCS OFF)

FetchContent_MakeAvailable(googleBenchMark)

add_executable(MatchingEngine)
add_executable(RandOrderGen)
add_executable(BenchMarkEngine)

target_sources(MatchingEngine PRIVATE 
    source/Main.cpp 
    source/OrderStore.cpp
    source/Logic.cpp
) 

target_sources(RandOrderGen PRIVATE
    source/RandOrderGen.cpp
    source/RandomOrder.cpp
)

target_sources(BenchMarkEngine PRIVATE
    benchmarks/BM_Engine.cpp
    source/OrderStore.cpp
    source/Logic.cpp
    source/RandomOrder.cpp
)

target_include_directories(MatchingEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(RandOrderGen PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(BenchMarkEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(BenchMarkEngine PRIVATE benchmark::benchmark)

