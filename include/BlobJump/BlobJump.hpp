#pragma once

#include <Blob/AssetManager.hpp>
#include <Game.hpp>

class BlobJump : public Game {

    Blob::AssetManager assetManager;

public:
    BlobJump(Blob::Core::Window &window, std::map<int, Player> &players);

    void run() final;
};
