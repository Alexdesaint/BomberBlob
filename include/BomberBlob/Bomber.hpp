#pragma once

#include <Core/DynamicCube.hpp>
#include <box2d/box2d.h>

#include <Blob/Core/Shape.hpp>

#include <BomberBlob/BombManager.hpp>
#include <BomberBlob/Textures.hpp>
#include <BomberBlob/UserData.hpp>

#include <Player.hpp>
#include <array>
#include <list>

class Bomber : public DynamicCube {
    friend BombManager;

private:
    // Box2d
    b2World &world;

    /*b2BodyDef bodyDef;
    b2PolygonShape polygonShape;
    b2FixtureDef fixtureDef;
    b2Body *body = nullptr;*/

    // Game
    Player &player;

    float acceleration = 2.5f, deceleration = 1.5f, maxSpeed = 2.f, bombPower = 2.f;
    bool alive = true, onBomb = false;

    unsigned int maxBomb = 1, bombPosed = 0;

    std::list<BombManager> &bombs;

    BombManager *lastBomb = nullptr;

    Blob::Materials::SingleColor material;
    Textures &textures;

public:
    Bomber(b2World &world, const Blob::Maths::Vec2<float> &pos, std::list<BombManager> &bombs, Player &player, Textures &textures,
           float scale = 0.8f);

    void updateInputs();

    void hit(Collider *c) final;

    [[nodiscard]] bool isAlive() const { return alive; }

    [[nodiscard]] Player &getPlayer() const;

    void drawInfo() const;
};
