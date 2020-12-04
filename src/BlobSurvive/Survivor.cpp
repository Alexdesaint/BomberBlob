#include <BlobSurvive/Survivor.hpp>

#include <numbers>

using namespace std;
using namespace Blob;

void Survivor::keyboardUpdate(const Blob::Core::Keyboard &keyboard) {
    speed = 0;

    if (keyboard.D)
        speed.y += 50;
    if (keyboard.A)
        speed.y -= 50;
    if (keyboard.W)
        speed.x -= 50;
    if (keyboard.S)
        speed.x += 50;
}
void Survivor::cursorPosUpdate(double xpos, double ypos) {
    xpos /= 400;
    ypos /= 400;

    if (ypos < -numbers::pi / 4)
        ypos = -numbers::pi / 4;
    if (ypos > numbers::pi / 2 - 0.01)
        ypos = numbers::pi / 2 - 0.01;

    direction.x = -cos(-xpos);
    direction.y = -sin(-xpos);
    cameraAngle.x = cos(ypos);
    cameraAngle.y = sin(ypos);
}
Survivor::Survivor(b2World &world, const Blob::Maths::Vec2<float> &pos, Function2D &groundFunction)
    : DynamicCollider(world, pos, 0.2, 0), groundFunction(groundFunction), position(pos, 0.2) {
    setScale({0.2, 0.2, 1});
    setPosition(position);
}
void Survivor::update() {
    if (!speed.isNull()) {
        b2Vec2 vel = body->GetLinearVelocity();
        b2Vec2 velChange = b2Vec2{speed.x * direction.x + speed.y * direction.y, speed.x * direction.y + speed.y * direction.x} - vel;
        b2Vec2 impulse = body->GetMass() * velChange;
        body->ApplyLinearImpulseToCenter(impulse, true);
    } else {
        body->SetLinearVelocity({0, 0});
    }

    b2Vec2 p = body->GetPosition();
    position = {p.x, p.y, (float) groundFunction.get(position.x, position.y) + 1};

    if (position.z < 0)
        position.z = 0;

    Maths::Vec3 cameraPos{direction * cameraAngle.x, cameraAngle.y};
    cameraPos = cameraPos * 15 + position;
    if (cameraPos.z < groundFunction.get(cameraPos.x, cameraPos.y) + 1)
        cameraPos.z = groundFunction.get(cameraPos.x, cameraPos.y) + 1;

    camera.setLookAt(position);
    camera.setPosition(cameraPos);

    setPosition(position);
    setRotation(body->GetAngle(), {0, 0, 1});
}
