cmake_minimum_required(VERSION 3.9)
set(LIB_NAME "BlobEngine")
project(BlobEngine VERSION 1.0.0 DESCRIPTION "Game utility tools")

set(CMAKE_CXX_STANDARD 11)

add_library(BlobEngine STATIC
        CollisionClass.hpp CollisionClass.cpp
        Geometrie.hpp Geometrie.cpp
        )