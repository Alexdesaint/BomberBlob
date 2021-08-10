#include <BomberBlob/Box.hpp>

#include <BomberBlob/UserData.hpp>

Box::Box(const Blob::Vec2<float> &pos, Blob::Material &material, b2World &world, float width)
    : DynamicCube(world, pos, width, material, UserData::BOX) {}

void Box::hit(Collider *c) {
    if (c->id == EXPLOSION)
        destroyed = true;
}
