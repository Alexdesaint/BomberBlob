#pragma once

#include <Core/DynamicCube.hpp>
#include <box2d/box2d.h>

class Explosion : public DynamicCube {
private:
    Blob::Vec2<float> initialPosition, direction;
    float maxSpeed = 8, distanceMax = 2;
    bool active = true;

    Blob::TextureAsset<"data/Explosion.bmp">::Instance texture;
    Blob::Materials::SingleTexture material{*texture, {4, 4}};

public:
    Explosion(b2World &world,
              const Blob::Vec2<float> &initialPosition,
              const Blob::Vec2<float> &direction,
              float distanceMax);

    bool keepMoving();

    void hit(Collider *c) final;

    [[nodiscard]] bool isActive() const { return active; }
};
