#pragma once

#include <Blob/Shapes.hpp>
#include <box2d/box2d.h>
#include <Core/Collider.hpp>

class StaticCube : public Blob::Shapes::Cube, public Collider {
public:
    // Box2d
    b2World &world;
    b2BodyDef bodyDef;
    b2PolygonShape polygonShape;
    b2Body *body = nullptr;

    StaticCube(b2World &world, const Blob::Maths::Vec2<float> &pos, float width, const Blob::Core::Material &material, unsigned int id = 0);
    StaticCube(b2World &world, const Blob::Maths::Vec3<float> &pos, float width, const Blob::Core::Material &material, unsigned int id = 0);
    StaticCube(b2World &world, const Blob::Maths::Vec3<float> &pos, const Blob::Maths::Vec3<float> &size, const Blob::Core::Material &material, unsigned int id = 0);
    ~StaticCube();
};