add_library(BomberBlob BomberBlob.cpp
		Bomb.cpp
		BombManager.cpp
		Box.cpp
		ContactListener.cpp
		DynamicCircle.cpp
		Explosion.cpp
		IndestructibleBox.cpp
		Player.cpp
		StaticCircle.cpp
		StaticRect.cpp
		)

target_link_libraries(BomberBlob sfml-system sfml-graphics sfml-window)
