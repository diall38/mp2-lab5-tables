# 'Google_test' is the subproject name
project(googletest)

# 'lib' is the folder with Google Test sources
add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# 'google_tests_run' is the target name
# 'test1.cpp tests2.cpp' are source files with tests
add_executable(google_tests_run ../tests/test_table.cpp ../include/table.h)
target_link_libraries(google_tests_run gtest gtest_main)