add_executable(BomberMan
		main.cpp
		Game.cpp
		Menu.cpp

        BomberBlob/BomberBlob.cpp
		BomberBlob/Bomb.cpp
		BomberBlob/Bonus.cpp
		BomberBlob/BombManager.cpp
		BomberBlob/Box.cpp
		BomberBlob/Explosion.cpp
		BomberBlob/IndestructibleBox.cpp
		BomberBlob/Bomber.cpp
        BomberBlob/Textures.cpp)

target_link_libraries(BomberMan Blob Blob::Collision Blob) # -static -static-libgcc -static-libstdc++-lpthread)
