#include <BomberBlob/Bonus.hpp>

#include <random>
#include <iostream>

std::random_device rd;
std::mt19937 mt(rd());
std::uniform_real_distribution<float> dist(0, 5);

Bonus::Bonus(Blob::Vec2f pos, Textures &textures, float scale) : RectStatic(pos, {scale, scale}, EXTRABOMB) {
    setPosition(pos.x, pos.y, scale/2.f);
	setScale(scale/2.f, scale/2.f, scale/2.f);

	auto val = (int)dist(mt);

	switch (val) {
        case 0:
            setObjectType(EXTRAPOWER);
            setMesh(textures.extraPower);
            break;
        case 1:
            setObjectType(EXTRASPEED);
            setMesh(textures.extraSpeed);
            break;
        case 2:
            setMesh(textures.extraBomb);
            break;
        default:
            destroyed = true;
            break;
    }

	//setTextureScale({4.f, 4.f});
}

bool Bonus::update() {
	return destroyed;
}

void Bonus::hit(int objectType, Object &object) {
	if (objectType == PLAYER) {
		destroyed = true;
	}
}

