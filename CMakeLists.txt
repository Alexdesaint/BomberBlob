cmake_minimum_required(VERSION 3.9)

project(BomberMan)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

find_package(SFML 2.5 COMPONENTS window graphics network audio system REQUIRED)
if (NOT SFML_FOUND)
	message(ERROR "SFML not found")
endif()

link_directories(../BlobEngine/lib)
include_directories(../BlobEngine/include)
include_directories(include)

subdirs(src)


