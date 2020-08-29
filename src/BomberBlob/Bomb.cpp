#include <BomberBlob/Bomb.hpp>

#include <BomberBlob/Bomber.hpp>

Bomb::Bomb(Blob::Vec2f pos, Blob::Mesh &mesh, float scale) : RectStatic(pos, {scale, scale}, BOMB),
                                                             Blob::Shape(mesh, pos.x, pos.y, scale / 2.f, scale / 2.f,
                                                                         scale / 2.f,
                                                                         scale / 2.f) {}

bool Bomb::isDestroyed() {
    return destroyed;
}

void Bomb::hit(int objectType, Object &object) {
    if (objectType == EXPLOSION) {
        destroyed = true;
    }
}