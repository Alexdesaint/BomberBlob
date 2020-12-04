#include <BomberBlob/Explosion.hpp>
#include <BomberBlob/UserData.hpp>

Explosion::Explosion(b2World &world, const Blob::Core::Material &material, const Blob::Maths::Vec2<float> &initialPosition,
                     const Blob::Maths::Vec2<float> &d, float distanceMax)
    : DynamicCube(world, initialPosition, 0.4f, material, UserData::EXPLOSION), distanceMax(distanceMax), initialPosition(initialPosition), direction(d) {

    speed = direction.normalize() * maxSpeed;

//    body->SetLinearVelocity({direction.x, direction.y});
}

void Explosion::hit(Collider *c) {
    if (c->id == INDESTRUCTIBLE_BOX || c->id == BOX || c->id == BOMB)
        active = false;
}

bool Explosion::keepMoving() {
    b2Vec2 p = body->GetPosition();
    Blob::Maths::Vec2<float> position = {p.x, p.y};
    if ((initialPosition - position).length2() > distanceMax * distanceMax) {
        active = false;
        return false;
    }

    return true;
}
