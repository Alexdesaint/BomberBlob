#pragma once

#include <Blob/Core/Camera.hpp>
#include <Blob/Core/Controls.hpp>
#include <Core/DynamicCollider.hpp>
#include <Core/PerlinNoise.hpp>

class Survivor : public DynamicCollider, public Blob::Shapes::Cube, private Blob::KeyboardEvents, private Blob::MouseEvents {
private:
    Blob::Materials::PBRSingleColor material;
    Function2D &groundFunction;

    Blob::Vec2<float> direction, cameraAngle, speed;
    Blob::Vec3<float> position;

    void keyboardUpdate(const Blob::Key &key) final;

    void cursorPosUpdate(double xpos, double ypos) final;

public:
    Blob::Camera camera{};

    Survivor(b2World &world, const Blob::Vec2<float> &pos, Function2D &groundFunction);

    void update() final;
};
