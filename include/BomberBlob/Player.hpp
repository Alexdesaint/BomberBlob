#ifndef BOMBERMAN_PLAYER_HPP
#define BOMBERMAN_PLAYER_HPP

#include <Blob/Collision/CollisionDetector.hpp>
#include <Blob/GL/Shapes.hpp>

#include <BomberBlob/UserData.hpp>

#include <array>
#include <list>

#include <BomberBlob/BombManager.hpp>

class Player : public Blob::Collision::RectDynamic, public Blob::GL::Shapes::Cube {
	friend BombManager;
public:
	enum Actions {
		up = 0,
		down,
		right,
		left,
		putBomb,
		numOfActions
	};
private:
	float maxSpeed = 2.5f, bombPower = 2.f;
	bool alive = true, onBomb = false, connected = false;

	std::string name;

	unsigned int maxBomb = 1, bombPosed = 0;

	const bool* keys[Actions::numOfActions];

	std::list<BombManager> &bombs;

	BombManager *lastBomb;

	Blob::GL::Texture texture;
public:

	Player(float x, float y, std::list<BombManager> &bombs, std::string name);

	void preCollisionUpdate() final;

	void postCollisionUpdate() final;

    void hit(int objectType, Object &object) final;

	void setAction(Actions a, const bool *key) {
		keys[a] = key;
	}

	bool isAlive() const {
		return alive;
	}

	float getMaxSpeed() const;

	float getBombPower() const;

	unsigned int getMaxBomb() const;

    bool isConnected() const;

    void setConnected(bool connected);

    const std::string &getName() const;

    void setName(const std::string &name);

    void drawInfo() const;
};


#endif //BOMBERMAN_PLAYER_HPP
