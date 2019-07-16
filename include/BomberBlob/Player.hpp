#ifndef BOMBERMAN_PLAYER_HPP
#define BOMBERMAN_PLAYER_HPP

#include <Blob/Collision/CollisionDetector.hpp>
#include <Blob/GL/Shapes.hpp>

#include <BomberBlob/UserData.hpp>
#include <BomberBlob/Textures.hpp>
#include <BomberBlob/BombManager.hpp>

#include <array>
#include <list>

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

	enum JoystickAxe {
	    x = 0,
	    y,
	    numOfAxes
	};
private:
	float acceleration = 2.5f, deceleration = 1.5f, maxSpeed = 2.5f, bombPower = 2.f;
	bool alive = true, onBomb = false, control = false;

	//deff
	const bool defbool = false;
	const unsigned char defchar = false;
	const float deffloat = 0;

	std::string name;

	unsigned int maxBomb = 1, bombPosed = 0;

	const bool* keys[Actions::numOfActions];

	const unsigned char* buttons[Actions::numOfActions];

    const float *joystickAxes[numOfAxes];

	std::list<BombManager> &bombs;

	BombManager *lastBomb;

	Textures &textures;
public:

	Player(float x, float y, std::list<BombManager> &bombs, std::string name, Textures &textures);

	void preCollisionUpdate() final;

	void postCollisionUpdate() final;

    void hit(int objectType, Object &object) final;

    void controller(bool c) {
        control = c;
    }

    void setJoystickAxe(JoystickAxe a, const float *joy) {
        joystickAxes[a] = joy;
    }

	void setKey(Actions a, const bool *key) {
		keys[a] = key;
	}

	void setButton(Actions a, const unsigned char *button) {
		buttons[a] = button;
	}

	bool isAlive() const {
		return alive;
	}

	void reset(float x, float y);

	float getMaxSpeed() const;

	float getBombPower() const;

	unsigned int getMaxBomb() const;

    const std::string &getName() const;

    void setName(const std::string &name);

    void drawInfo() const;
};


#endif //BOMBERMAN_PLAYER_HPP
