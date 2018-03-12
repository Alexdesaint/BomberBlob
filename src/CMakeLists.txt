subdirs(BlobEngine)

add_executable(BomberMan
        main.cpp
        BomberMan.cpp BomberMan.hpp)

target_link_libraries(BomberMan BlobEngine ${SFML_LIBRARIES};${SFML_DEPENDENCIES} -static-libgcc -static-libstdc++ -static -lpthread)
