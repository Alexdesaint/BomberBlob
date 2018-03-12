cmake_minimum_required(VERSION 3.9)

project(BomberMan)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

set(CMAKE_MODULE_PATH "C:/msys64/mingw64/cmake/Modules")

set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML REQUIRED window graphics network audio system)
IF (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    message("SFML found")
else()
    message(WARNING "SFML not found")
endif()

subdirs(src)


