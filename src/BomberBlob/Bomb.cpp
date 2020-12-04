#include <BomberBlob/Bomb.hpp>

#include <BomberBlob/UserData.hpp>

Bomb::Bomb(const Blob::Maths::Vec2<float> &pos, Blob::Core::Material &material, b2World &world, float width)
    : DynamicCube(world, pos, width, material, UserData::BOMB) {}

bool Bomb::isDestroyed() const {
    return destroyed;
}

void Bomb::hit(int objectType, void *object) {
    if (objectType == EXPLOSION) {
        destroyed = true;
    }
}
