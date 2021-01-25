#pragma once

#include <Blob/AssetManager.hpp>
#include <Core/StaticCube.hpp>
#include <Game.hpp>
#include <box2d/box2d.h>

class BlobJump : public Game {

    Blob::AssetManager assetManager;
    b2World world;
    Blob::Core::Scene scene;
    std::list<StaticCube> staticCubes;

    bool loaded = false;

public:
    BlobJump(Blob::Core::Window &window, std::map<int, Player> &players);

    bool loadReady() final {
        window.draw(scene);
        return !loaded;
    };

    bool gameReady() final { return loaded; };

    void load() final;

    void run() final;
};
