#ifndef BOMBERMAN_BOMBERMAN_HPP
#define BOMBERMAN_BOMBERMAN_HPP

#include <BlobEngine/BlobGL/Graphic.hpp>

#include "BombManager.hpp"
#include "IndestructibleBox.hpp"
#include "Box.hpp"
#include "Bonus.hpp"

class BomberBlob {
private:
	std::list<BombManager> bombs;

	const std::array<bool, Blob::GL::Key::KeyCount> &keys = Blob::GL::Graphic::getKeys();

	int width = 21, height = 15;

	Blob::GL::Shapes::Plane ground;

	std::list<IndestructibleBox> indestructibleBoxs;
	std::list<Box> boxs;
	std::list<Bonus> bonus;

	Blob::GL::Texture speedTex,	explosionRangeTex, maxBombTex;

	Blob::Collision::CollisionDetector collisionDetector{};

public:
	explicit BomberBlob(Blob::GL::Graphic &window);
};

#endif //BOMBERMAN_BOMBERMAN_HPP
