#pragma once

#include "Blob/Core/Texture.hpp"
#include <Core/DynamicCube.hpp>
#include <box2d/box2d.h>

class Player;

class Bomb : public DynamicCube {
private:
    bool destroyed = false;

    Blob::TextureAsset<"data/Bomb.bmp">::Instance texture;
    Blob::Materials::SingleTexture material{*texture, {4, 4}};

public:
    explicit Bomb(const Blob::Vec2<float> &pos,
                  b2World &world,
                  float width = 0.4f);

    void hit(int objectType, void *object);

    [[nodiscard]] bool isDestroyed() const;
};
