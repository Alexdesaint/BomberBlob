#ifndef BOMBERMAN_BOMBMANAGER_HPP
#define BOMBERMAN_BOMBMANAGER_HPP

#include <BomberBlob/Bomb.hpp>
#include <BomberBlob/Explosion.hpp>

#include <BlobEngine/BlobGL/Shape.hpp>

#include <BlobEngine/Time.hpp>

class Player;

class BombManager : public Blob::GL::Shape {
private:
	Bomb *bomb;
	float bombDelay = 3;

	Explosion *exUP, *exDO, *exRI, *exLE;

	Blob::Time::TimePoint start;

	Player &player;

public:
	explicit BombManager(Blob::Vec2f pos, Player &player);

	Bomb *getBomb() const;

	bool update();

	~BombManager();
};

#endif //BOMBERMAN_BOMBMANAGER_HPP
