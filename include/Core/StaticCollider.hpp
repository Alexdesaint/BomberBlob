#pragma once

#include <Blob/Shapes.hpp>
#include <box2d/box2d.h>
#include <Core/Collider.hpp>

class StaticCollider : public Collider {
public:
    // Box2d
    b2World &world;
    b2BodyDef bodyDef;
    b2PolygonShape polygonShape;
    b2Body *body = nullptr;

    StaticCollider(b2World &world, const Blob::Vec2<float> &pos, const Blob::Vec2<float> &size, unsigned int id = 0);
    ~StaticCollider();
};