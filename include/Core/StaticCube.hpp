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

    StaticCube(b2World &world, const Blob::Vec2<float> &pos, float width, Blob::Material &material, unsigned int id = 0);
    StaticCube(b2World &world, const Blob::Vec3<float> &pos, float width, Blob::Material &material, unsigned int id = 0);
    StaticCube(b2World &world, const Blob::Vec3<float> &pos, const Blob::Vec3<float> &size, Blob::Material &material, unsigned int id = 0);
    ~StaticCube();
};