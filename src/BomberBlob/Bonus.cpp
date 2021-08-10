#include <BomberBlob/Bonus.hpp>

#include <BomberBlob/UserData.hpp>

#include <iostream>
#include <random>

std::random_device rd;
std::mt19937 mt(rd());
std::uniform_real_distribution<float> dist(0, 5);

Bonus::Bonus(const Blob::Vec2<float> &pos, Textures &textures, b2World &world, float width)
    : StaticCube(world, pos, width, textures.defaultMat) {

    auto val = (int) dist(mt);

    switch (val) {
    case 0:
        id = EXTRAPOWER;
        primitive0.setMaterial(&textures.extraPowerMat);
        break;
    case 1:
        id = EXTRASPEED;
        primitive0.setMaterial(&textures.extraSpeedMat);
        break;
    case 2:
        id = EXTRABOMB;
        primitive0.setMaterial(&textures.extraBombMat);
        break;
    default:
        destroyed = true;
        break;
    }

    bodyDef.position.Set(pos.x, pos.y);
    body = world.CreateBody(&bodyDef);

    polygonShape.SetAsBox(width, width);
    body->CreateFixture(&polygonShape, 0.0f);

    setMesh(mesh);
    setPosition({pos, 0});
    setScale(width);
}

bool Bonus::update() const {
    return destroyed;
}

void Bonus::hit(Collider *c) {
    if (c->id == PLAYER) {
        destroyed = true;
    }
}
