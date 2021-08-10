#pragma once

#include <BomberBlob/Bomb.hpp>
#include <BomberBlob/Explosion.hpp>
#include <BomberBlob/Textures.hpp>

#include <Blob/Core/Shape.hpp>

#include <Blob/Time.hpp>

class Bomber;

class BombManager : public Blob::Shape {
private:
    Bomb *bomb;
    float bombDelay = 3;

    Explosion *exUP = nullptr, *exDO = nullptr, *exRI = nullptr, *exLE = nullptr;

    Blob::Time::TimePoint start;

    Bomber &bomber;

    Textures &textures;
    b2World &world;
public:
    explicit BombManager(b2World &world, const Blob::Vec2<float> &pos, Bomber &bomber, Textures &textures);

    ~BombManager();

    Bomb *getBomb() const;

    bool update();
};
