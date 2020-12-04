#pragma once

#include <Blob/Shapes.hpp>
#include <box2d/box2d.h>
#include <unordered_set>
#include <Blob/Maths.inl>
#include <Core/Collider.hpp>

class DynamicCube : public Blob::Shapes::Cube, public Collider {
private:
    static std::unordered_set<DynamicCube*> cubes;
public:

    // Box2d
    b2World &world;
    b2BodyDef bodyDef;
    b2PolygonShape polygonShape;
    b2FixtureDef fixtureDef;
    b2Body *body = nullptr;

    // Blob
    Blob::Maths::Vec2<float> speed{}, position{};

    explicit DynamicCube(b2World &world, const Blob::Maths::Vec2<float> &position, float width, const Blob::Core::Material &material, unsigned int id = 0);

    ~DynamicCube();

    void update();

    static void updateAll() {
        for(auto &c : cubes)
            c->update();
    }
};