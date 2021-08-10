#pragma once

#include <box2d/box2d.h>
#include <Core/DynamicCube.hpp>

class Player;

class Bomb : public DynamicCube {
private:
    bool destroyed = false;
public:
    explicit Bomb(const Blob::Vec2<float>& pos, Blob::Material &material, b2World &world, float width = 0.4f);

    void hit(int objectType, void *object);

    [[nodiscard]] bool isDestroyed() const;
};
