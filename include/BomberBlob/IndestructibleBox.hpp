#pragma once

#include <box2d/box2d.h>
#include <Core/StaticCube.hpp>

class IndestructibleBox : public StaticCube {
private:
    bool destroyed = false;

public:
    IndestructibleBox(const Blob::Maths::Vec2<float> &pos, Blob::Core::Material &material, b2World &world, float width = 1.0f);
};
