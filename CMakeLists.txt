cmake_minimum_required(VERSION 4.1)
project(SS02)

set(CMAKE_CXX_STANDARD 20)


add_executable(SS02 main.cpp
        UsingStructures/UsingStructures.cpp
        UsingEnumerations/UsingEnumerations.cpp
        UsingPointersAndTheFreeStore/UsingPointersAndTheFreeStore.cpp
        KeyboardWhackEmAll/KeyboardWhackEmAll.cpp
        KeyboardWhackEmAll/Windows.cpp
        KeyboardWhackEmAll/Windows.h)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})
target_link_libraries(SS02 ${CURSES_LIBRARY})


