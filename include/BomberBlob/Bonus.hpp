#pragma once

#include <BomberBlob/Textures.hpp>
#include <Core/StaticCube.hpp>
#include <box2d/box2d.h>

class Bonus : public StaticCube {
private:
    bool destroyed = false;

public:
    Bonus(const Blob::Vec2<float> &pos, Textures &textures, b2World &world, float width = 0.4f);

    void hit(Collider *c) final;

    [[nodiscard]] bool update() const;
};
