#pragma once

#include <box2d/box2d.h>
#include <Core/DynamicCube.hpp>

class Box : public DynamicCube {
private:
    bool destroyed = false;

public:
    explicit Box(const Blob::Maths::Vec2<float>& pos, Blob::Core::Material &material, b2World &world, float width = 0.8f);

    void hit(Collider *c) final;

    [[nodiscard]] bool isDestroy() const { return destroyed; }
};
