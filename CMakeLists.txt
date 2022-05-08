cmake_minimum_required(VERSION 3.18)
project(mp2-lab5-tables)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)
include_directories(include)


add_executable(mp2-lab5-tables include/polynomial.h include/table.h samples/main.cpp)
add_subdirectory(google_test)