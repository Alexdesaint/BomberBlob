#include <Core/DynamicCollider.hpp>

std::unordered_set<DynamicCollider *> DynamicCollider::cubes;

DynamicCollider::DynamicCollider(b2World &world, const Blob::Vec2<float> &position, float width, unsigned int id)
    : Collider(id), world(world) {
    //std::lock_guard<std::mutex> guard(mutex);
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(position.x, position.y);
    bodyDef.userData = this;
    body = world.CreateBody(&bodyDef);

    polygonShape.SetAsBox(width, width);

    fixtureDef.shape = &polygonShape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;

    body->CreateFixture(&fixtureDef);

    cubes.emplace(this);
}

DynamicCollider::~DynamicCollider() {
    //std::lock_guard<std::mutex> guard(mutex);
    world.DestroyBody(body);
    cubes.erase(this);
}

void DynamicCollider::update() {

}
