cmake_minimum_required(VERSION 3.9)

project(BomberMan)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

ExternalProject_Add()

link_directories(../BlobEngine/lib)
include_directories(../BlobEngine/include)
include_directories(include)

subdirs(src)


