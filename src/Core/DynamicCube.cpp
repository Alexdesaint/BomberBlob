#include <Core/DynamicCube.hpp>

std::unordered_set<DynamicCube *> DynamicCube::cubes;

DynamicCube::DynamicCube(b2World &world, const Blob::Maths::Vec2<float> &position, float width, const Blob::Core::Material &material, unsigned int id)
    : Collider(id), Blob::Shapes::Cube(material), position(position), world(world) {
    std::lock_guard<std::mutex> guard(mutex);
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(position.x, position.y);
    bodyDef.userData = this;
    {
        std::lock_guard<std::mutex> guard(mutex);
        body = world.CreateBody(&bodyDef);
    }

    polygonShape.SetAsBox(width, width);

    fixtureDef.shape = &polygonShape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;

    body->CreateFixture(&fixtureDef);

    setPosition({position, width});
    setScale(width);

    cubes.emplace(this);
}

DynamicCube::~DynamicCube() {
    std::lock_guard<std::mutex> guard(mutex);
    world.DestroyBody(body);
    cubes.erase(this);
}

void DynamicCube::update() {
    std::lock_guard<std::mutex> guard(mutex);
    if (!speed.isNull()) {
        b2Vec2 vel = body->GetLinearVelocity();
        b2Vec2 velChange = b2Vec2{speed.x, speed.y} - vel;
        b2Vec2 impulse = body->GetMass() * velChange;
        body->ApplyLinearImpulseToCenter(impulse, true);
    }

    b2Vec2 p = body->GetPosition();
    position = {p.x, p.y};
    float angle = body->GetAngle();
    setPosition(position);
    setRotation(angle, {0, 0, 1});
}
