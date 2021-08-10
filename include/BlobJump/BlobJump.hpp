#pragma once

#include <Core/StaticCube.hpp>
#include <Game.hpp>
#include <box2d/box2d.h>

class BlobJump : public Game {

    b2World world;
    Blob::Scene scene;
    std::list<StaticCube> staticCubes;

    bool loaded = false;

public:
    BlobJump(Blob::Window &window, std::map<int, Player> &players);

    bool loadReady() final {
        window.draw(scene);
        return !loaded;
    };

    bool gameReady() final { return loaded; };

    void load() final;

    void run() final;
};
