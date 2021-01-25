#pragma once

#include <Blob/Core/Camera.hpp>
#include <Blob/Core/Controls.hpp>
#include <Core/DynamicCollider.hpp>
#include <Core/PerlinNoise.hpp>

class Survivor : public DynamicCollider, public Blob::Shapes::Cube, private Blob::Core::KeyboardEvents, private Blob::Core::MouseEvents {
private:
    Blob::Materials::PBRSingleColor material;
    Function2D &groundFunction;

    Blob::Maths::Vec2<float> direction, cameraAngle, speed;
    Blob::Maths::Vec3<float> position;

    void keyboardUpdate(const Blob::Core::Key &key) final;

    void cursorPosUpdate(double xpos, double ypos) final;

public:
    Blob::Core::Camera camera{};

    Survivor(b2World &world, const Blob::Maths::Vec2<float> &pos, Function2D &groundFunction);

    void update() final;
};
