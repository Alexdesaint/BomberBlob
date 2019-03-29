#include <BomberBlob/Player.hpp>

using namespace Blob;

Player::Player(float x, float y, std::list<BombManager> &bombs) : RectDynamic({x, y}, {0.8f, 0.8f}, PLAYER),
                                                                  bombs(bombs) {
    Cube::setPosition(x, y, 0.4f);
	setScale(0.8f, 0.8f, 0.8f);

	setColor(255, 255, 255);
}

void Player::preCollisionUpdate() {
	Vec2f Acceleration;

	if (*keys[Actions::left]) {
		Acceleration.x -= 1;
	}
	if (*keys[Actions::right]) {
		Acceleration.x += 1;
	}
	if (*keys[Actions::down]) {
		Acceleration.y -= 1;
	}
	if (*keys[Actions::up]) {
		Acceleration.y += 1;
	}

	if (!Acceleration.isNull()) {
		speed = Acceleration.setLength(maxSpeed);
	} else
		speed.reset();

	if (*keys[Actions::putBomb] && !onBomb && bombPosed < maxBomb) {
        bombs.emplace_front(getPosition(), *this);
		lastBomb = &bombs.front();
		onBomb = true;
		bombPosed++;
	}
}

void Player::postCollisionUpdate() {
    Cube::setPosition(getPosition(), 0.4f);

	if (onBomb) {
		Bomb *bomb = lastBomb->getBomb();
		if (bomb == nullptr || !bomb->overlap(*this)) {
			onBomb = false;
		}
	}
}

void Player::hit(const int objectType, Object &object) {
    if (onBomb && objectType == BOMB && &object == lastBomb->getBomb()) {
        setReaction(IGNORE);
        return;
	}
	if (objectType == EXTRABOMB) {
		maxBomb++;
	} else if (objectType == EXTRASPEED) {
		maxSpeed += 1;
	} else if (objectType == EXTRAPOWER) {
		bombPower++;
	} else if (objectType == EXPLOSION) {
		alive = false;
        setReaction(IGNORE);
        return;
    }

    setReaction(STOP);
}

float Player::getMaxSpeed() const {
	return maxSpeed;
}

float Player::getBombPower() const {
	return bombPower;
}

unsigned int Player::getMaxBomb() const {
	return maxBomb;
}
