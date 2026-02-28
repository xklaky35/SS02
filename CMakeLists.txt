cmake_minimum_required(VERSION 4.1)
project(SS02)

set(CMAKE_CXX_STANDARD 20)

add_executable(SS02 main.cpp
        UsingStructures/UsingStructures.cpp
        UsingEnumerations/UsingEnumerations.cpp
        UsingPointersAndTheFreeStore/UsingPointersAndTheFreeStore.cpp)
