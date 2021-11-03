#pragma once

#include <Core/StaticCube.hpp>
#include <box2d/box2d.h>

class IndestructibleBox : public StaticCube {
private:
    bool destroyed = false;

    Blob::TextureAsset<"data/IndestructibleBox.bmp">::Instance texture;
    Blob::Materials::SingleTexture material{*texture, {4, 4}};

public:
    IndestructibleBox(const Blob::Vec2<float> &pos,
                      b2World &world,
                      float width = 1.0f);
};
