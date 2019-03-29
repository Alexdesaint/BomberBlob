#include <BomberBlob/Bomb.hpp>

#include <BomberBlob/Player.hpp>

Bomb::Bomb(Blob::Vec2f pos) : RectStatic(pos, {0.4f, 0.4f}, BOMB) {

    Cube::setPosition(pos.x, pos.y, 0.2f);
	setScale(0.4f, 0.4f, 0.4f);

	loadBMP("data/Bomb.bmp");
}

bool Bomb::isDestroyed() {
	return destroyed;
}

void Bomb::hit(int objectType, Object &object) {
	if(objectType == EXPLOSION) {
		destroyed = true;
	}
}