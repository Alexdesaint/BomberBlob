#include <BomberBlob/Bonus.hpp>

#include <random>
#include <iostream>

std::random_device rd;
std::mt19937 mt(rd());
std::uniform_real_distribution<float> dist(0, 5);

Bonus::Bonus(Blob::Vec2f pos, Textures &textures) : RectStatic(pos, {0.4f, 0.4f}, EXTRABOMB) {
    Cube::setPosition(pos.x, pos.y, 0.2f);
	setScale(0.4f, 0.4f, 0.4f);

	auto val = (int)dist(mt);

	switch (val) {
        case 0:
            setObjectType(EXTRAPOWER);
            setTexture(textures.extraPower);
            break;
        case 1:
            setObjectType(EXTRASPEED);
            setTexture(textures.extraSpeed);
            break;
        case 2:
            setTexture(textures.extraBomb);
            break;
        default:
            destroyed = true;
            break;
    }

	setTextureScale({4.f, 4.f});
}

bool Bonus::update() {
	return destroyed;
}

void Bonus::hit(int objectType, Object &object) {
	if (objectType == PLAYER) {
		destroyed = true;
	}
}

