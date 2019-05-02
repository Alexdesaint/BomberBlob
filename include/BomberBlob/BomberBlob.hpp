#ifndef BOMBERMAN_BOMBERMAN_HPP
#define BOMBERMAN_BOMBERMAN_HPP

#include <Blob/GL/Graphic.hpp>

#include "BombManager.hpp"
#include "IndestructibleBox.hpp"
#include "Box.hpp"
#include "Bonus.hpp"
#include "Player.hpp"

class BomberBlob {
private:
    Blob::GL::Graphic &window;

	std::list<BombManager> bombs;
    std::unordered_map<int, Player> players;

	int width, height;

	Blob::GL::Shapes::Plane ground;

	std::list<IndestructibleBox> indestructibleBoxs;
	std::list<Box> boxs;
	std::list<Bonus> bonus;

	Blob::Collision::CollisionDetector collisionDetector{};

public:
    explicit BomberBlob(Blob::GL::Graphic &window);

	void gameLoop();
};

#endif //BOMBERMAN_BOMBERMAN_HPP
