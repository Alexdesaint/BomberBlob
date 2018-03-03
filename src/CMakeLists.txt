add_executable(${EXECUTABLE_NAME}
        main.cpp
        BomberMan.cpp BomberMan.hpp)

subdirs(BlobEngine)
set(CMAKE_MODULE_PATH "C:/msys64/mingw64/cmake/Modules")

set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML REQUIRED window graphics network audio system)
IF (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    message("SFML found")
else()
    message(WARNING "SFML not found")
endif()

message("Include : ${FIND_SFML_PATHS}")
message("Include : ${SFML_INCLUDE_DIR}")

target_link_libraries(${EXECUTABLE_NAME} BlobEngine ${SFML_LIBRARIES};${SFML_DEPENDENCIES} -static-libgcc -static-libstdc++ -static -lpthread)