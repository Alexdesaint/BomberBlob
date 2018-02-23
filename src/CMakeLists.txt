add_executable(${EXECUTABLE_NAME}
        main.cpp
        CollisionClass.hpp CollisionClass.cpp
        Geometrie.hpp Geometrie.cpp
        )

find_package(SFML REQUIRED system window graphics network audio)
IF (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    message("SFML found")
else()
    message(WARNING "SFML not found")
endif()

message("Include : ${SFML_LIBRARIES}")

target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES})
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")