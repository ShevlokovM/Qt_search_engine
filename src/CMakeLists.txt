cmake_minimum_required(VERSION 3.5)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

include_directories(../include)

include(FetchContent)

FetchContent_Declare(json
        GIT_REPOSITORY https://github.com/nlohmann/json
        GIT_TAG v3.10.5
        )
FetchContent_MakeAvailable(json)

set(PROJECT_SOURCES
        ../include/invertedindex.h
        ../include/searchserver.h
        ../include/converterjson.h
        ../include/poolofthreads.h
        ../include/mainwindow.h
	main.cpp
        mainwindow.cpp
        mainwindow.ui
        invertedindex.cpp
        searchserver.cpp
        converterjson.cpp
        poolofthreads.cpp
        caller.ui        
)



if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(search_engine_v5_exe
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET search_engine_v5 APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(search_engine_v5_exe SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(search_engine_v5_exe
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(search_engine_v5_exe PRIVATE Qt${QT_VERSION_MAJOR}::Widgets nlohmann_json::nlohmann_json)

set_target_properties(search_engine_v5_exe PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(search_engine_v5_exe)
endif()
