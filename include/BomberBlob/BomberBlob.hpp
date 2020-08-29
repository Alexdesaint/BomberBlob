#pragma once

#include <Blob/Window.hpp>
#include <Blob/Shapes.hpp>

#include <Game.hpp>

#include <BomberBlob/Explosion.hpp>
#include <BomberBlob/Bomber.hpp>
#include <BomberBlob/IndestructibleBox.hpp>
#include <BomberBlob/Box.hpp>
#include <BomberBlob/BombManager.hpp>
#include <BomberBlob/Bonus.hpp>

class BomberBlob : public Game {
private:
    std::list<BombManager> bombs;

    int width = 21, height = 15;
	Textures &textures;

    Blob::Shape ground;

    Blob::Camera camera;

    std::list<IndestructibleBox> indestructibleBoxes;
    std::list<Box> boxes;
    std::list<Bonus> bonus;

    Blob::Collision::CollisionDetector collisionDetector{};

	std::list<Bomber> bombers;

	void gameLoop();

public:
    BomberBlob(Blob::Window &window, std::map<int, Player> &players, Textures &textures);

    void settings();

    void start();
};
