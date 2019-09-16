#pragma once

#include <Blob/Collision/CollisionDetector.hpp>
#include <Blob/GL/Shapes.hpp>

#include <BomberBlob/UserData.hpp>
#include <BomberBlob/Textures.hpp>
#include <BomberBlob/BombManager.hpp>

#include <array>
#include <list>
#include <Player.hpp>

class Bomber : public Blob::Collision::RectDynamic, public Blob::GL::Shapes::Cube {
	friend BombManager;
private:
	Player &player;

	float acceleration = 2.5f, deceleration = 1.5f, maxSpeed = 2.f, bombPower = 2.f;
	bool alive = true, onBomb = false;

	unsigned int maxBomb = 1, bombPosed = 0;

	std::list<BombManager> &bombs;

	BombManager *lastBomb = nullptr;

	Textures &textures;
public:

	Bomber(Blob::Vec2f pos, std::list<BombManager> &bombs, Player &player, Textures &textures);

	void preCollisionUpdate() final;

	void postCollisionUpdate() final;

    void hit(int objectType, Object &object) final;

	bool isAlive() const {
		return alive;
	}

	float getMaxSpeed() const;

	float getBombPower() const;

	unsigned int getMaxBomb() const;

	Player &getPlayer() const;

	void drawInfo() const;
};
