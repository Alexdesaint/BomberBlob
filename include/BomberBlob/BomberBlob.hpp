#ifndef BOMBERMAN_BOMBERMAN_HPP
#define BOMBERMAN_BOMBERMAN_HPP

#include <Blob/GL/Graphic.hpp>

#include <BomberBlob/Explosion.hpp>
#include <BomberBlob/Player.hpp>
#include <BomberBlob/IndestructibleBox.hpp>
#include <BomberBlob/Box.hpp>
#include <BomberBlob/BombManager.hpp>
#include <BomberBlob/Bonus.hpp>

class BomberBlob {
private:
    Textures textures;
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
