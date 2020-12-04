add_library(BomberBlob STATIC
        BomberBlob.cpp
        Bomb.cpp
        Bonus.cpp
        BombManager.cpp
        Box.cpp
        Explosion.cpp
        IndestructibleBox.cpp
        Bomber.cpp
        Textures.cpp)

target_link_libraries(BomberBlob Core)

