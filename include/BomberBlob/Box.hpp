#pragma once

#include <Core/DynamicCube.hpp>
#include <box2d/box2d.h>

class Box : public DynamicCube {
private:
    bool destroyed = false;

    Blob::TextureAsset<"data/Box.bmp">::Instance texture;
    Blob::Materials::SingleTexture material{*texture, {4, 4}};

public:
    explicit Box(const Blob::Vec2<float> &pos,
                 b2World &world,
                 float width = 0.8f);

    void hit(Collider *c) final;

    [[nodiscard]] bool isDestroy() const { return destroyed; }
};
