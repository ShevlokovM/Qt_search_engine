cmake_minimum_required(VERSION 3.5)

project(GoogleTest VERSION 0.1 LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.8.0
)

include_directories(${gtest_SOURCE_DIR} ${gtest_SOURCE_DIR}/include)

include_directories(../include
                   ../src
)

set(gtest_disable_pthreads on)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(GoogleTest tst_invertedindex.cpp tst_searchserver.cpp
    ../src/invertedindex.cpp ../src/searchserver.cpp ../src/poolofthreads.cpp)

target_link_libraries(GoogleTest PRIVATE gtest gtest_main)

include(GoogleTest)

gtest_discover_tests(GoogleTest)

