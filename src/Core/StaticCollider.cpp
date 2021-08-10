#include <Core/StaticCollider.hpp>
StaticCollider::StaticCollider(b2World &world, const Blob::Vec2<float> &pos, const Blob::Vec2<float> &size, unsigned int id)
    : Collider(id), world(world) {
    //std::lock_guard<std::mutex> guard(mutex);
    bodyDef.position.Set(pos.x, pos.y);
    bodyDef.userData = this;
    body = world.CreateBody(&bodyDef);

    polygonShape.SetAsBox(size.x, size.y);
    body->CreateFixture(&polygonShape, 0.0f);
}

StaticCollider::~StaticCollider() {
    //std::lock_guard<std::mutex> guard(mutex);
    world.DestroyBody(body);
}
