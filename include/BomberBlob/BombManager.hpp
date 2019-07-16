#ifndef BOMBERMAN_BOMBMANAGER_HPP
#define BOMBERMAN_BOMBMANAGER_HPP

#include <BomberBlob/Bomb.hpp>
#include <BomberBlob/Explosion.hpp>
#include <BomberBlob/Textures.hpp>

#include <Blob/GL/Shape.hpp>

#include <Blob/Time.hpp>

class Player;

class BombManager : public Blob::GL::Shape {
private:
	Bomb *bomb;
	float bombDelay = 3;

	Explosion *exUP, *exDO, *exRI, *exLE;

	Blob::Time::TimePoint start;

	Player &player;

    Textures &textures;

public:
	explicit BombManager(Blob::Vec2f pos, Player &player, Textures &textures);

    ~BombManager();

	Bomb *getBomb() const;

	bool update();
};

#endif //BOMBERMAN_BOMBMANAGER_HPP
