cmake_minimum_required(VERSION 3.5)

project(searchenginetest LANGUAGES CXX)

enable_testing()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Gui Test REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Gui Test REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#include_directories(search_engine)

add_executable(searchenginetest tst_searchenginetest.cpp)
#target_link_libraries(searchenginetest PRIVAT search_engine/searchserver.h)
target_include_directories(searchenginetest PUBLIC search_engine)
add_test(NAME searchenginetest COMMAND searchenginetest)
#target_include_directories(searchenginetest PUBLIC ${MY_INCLUDE_DIR})
#include_directories(search_engine)

target_link_libraries(searchenginetest PRIVATE Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Test)

