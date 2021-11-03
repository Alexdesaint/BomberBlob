#pragma once

#include <Blob/Maths.inl>
#include <Blob/Shapes.hpp>
#include <Core/Collider.hpp>
#include <box2d/box2d.h>
#include <unordered_set>

class DynamicCollider : public Collider {
private:
    static std::unordered_set<DynamicCollider *> cubes;

public:
    // Box2d
    b2World &world;
    b2BodyDef bodyDef;
    b2PolygonShape polygonShape;
    b2FixtureDef fixtureDef;
    b2Body *body = nullptr;

    explicit DynamicCollider(b2World &world,
                             const Blob::Vec2<float> &position,
                             float width,
                             unsigned int id = 0);

    ~DynamicCollider();

    virtual void update();

    static void updateAll() {
        for (auto &c : cubes)
            c->update();
    }
};