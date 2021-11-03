#pragma once

#include "Blob/Color.hpp"
#include <Core/StaticCube.hpp>
#include <Game.hpp>
#include <box2d/box2d.h>

class BlobJump : public Game {

    b2World world;
    Blob::Scene scene;
    std::list<StaticCube> staticCubes;
    Blob::Materials::SingleColor defaultMat{Blob::Color::Green};

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
