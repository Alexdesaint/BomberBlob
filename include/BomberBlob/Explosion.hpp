#pragma once

#include <box2d/box2d.h>
#include <Core/DynamicCube.hpp>

class Explosion : public DynamicCube {
private:
    Blob::Vec2<float> initialPosition, direction;
    float maxSpeed = 8, distanceMax = 2;
    bool active = true;

public:
    Explosion(b2World &world, const Blob::Material &material, const Blob::Vec2<float> &initialPosition, const Blob::Vec2<float> &direction, float distanceMax);

    bool keepMoving();

    void hit(Collider *c) final;

    [[nodiscard]] bool isActive() const { return active; }
};
