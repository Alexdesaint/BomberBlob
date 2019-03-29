add_executable(BomberMan
		main.cpp
		BomberBlob/GameMenu.cpp
		BomberBlob/BomberBlob.cpp

		BomberBlob/Bomb.cpp
		BomberBlob/Bonus.cpp
		BomberBlob/BombManager.cpp
		BomberBlob/Box.cpp
		BomberBlob/Explosion.cpp
		BomberBlob/IndestructibleBox.cpp
		BomberBlob/Player.cpp
		BomberBlob/InfoBar.cpp
		)

target_link_libraries(BomberMan Blob::GL Blob::Collision Blob::Geometrie) # -static -static-libgcc -static-libstdc++-lpthread)
