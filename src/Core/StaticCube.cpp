#include <Core/StaticCube.hpp>

StaticCube::StaticCube(b2World &world, const Blob::Maths::Vec2<float> &pos, float width, const Blob::Core::Material &material, unsigned int id)
    : Blob::Shapes::Cube(material), Collider(id), world(world) {
    //std::lock_guard<std::mutex> guard(mutex);
    bodyDef.position.Set(pos.x, pos.y);
    bodyDef.userData = this;
    body = world.CreateBody(&bodyDef);

    polygonShape.SetAsBox(width, width);
    body->CreateFixture(&polygonShape, 0.0f);
    // body->Us(this);

    setMesh(mesh);
    setPosition({pos, width});
    setScale(width);
}
StaticCube::StaticCube(b2World &world, const Blob::Maths::Vec3<float> &pos, float width, const Blob::Core::Material &material, unsigned int id)
    : Blob::Shapes::Cube(material), Collider(id), world(world) {
    //std::lock_guard<std::mutex> guard(mutex);
    bodyDef.position.Set(pos.x, pos.y);
    bodyDef.userData = this;
    body = world.CreateBody(&bodyDef);

    polygonShape.SetAsBox(width, width);
    body->CreateFixture(&polygonShape, 0.0f);
    // body->Us(this);

    setMesh(mesh);
    setPosition({pos.x, pos.y, pos.z + width});
    setScale(width);
}
StaticCube::StaticCube(b2World &world, const Blob::Maths::Vec3<float> &pos, const Blob::Maths::Vec3<float> &size,
                       const Blob::Core::Material &material, unsigned int id)
    : Blob::Shapes::Cube(material), Collider(id), world(world) {
    //std::lock_guard<std::mutex> guard(mutex);
    bodyDef.position.Set(pos.x, pos.y);
    bodyDef.userData = this;
    body = world.CreateBody(&bodyDef);

    polygonShape.SetAsBox(size.x, size.y);
    body->CreateFixture(&polygonShape, 0.0f);
    // body->Us(this);

    setMesh(mesh);
    setPosition({pos.x, pos.y, pos.z + size.z});
    setScale(size);
}

StaticCube::~StaticCube() {
    //std::lock_guard<std::mutex> guard(mutex);
    world.DestroyBody(body);
}