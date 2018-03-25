add_executable(BomberMan
        main.cpp
        BomberMan.cpp BomberMan.hpp)

message(${SFML_LIBRARIES};${SFML_DEPENDENCIES})
target_link_libraries(BomberMan Geometrie Collision ${SFML_LIBRARIES};${SFML_DEPENDENCIES} -static-libgcc -static-libstdc++ -static -lpthread)
