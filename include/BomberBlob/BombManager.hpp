#ifndef BOMBERMAN_BOMBMANAGER_HPP
#define BOMBERMAN_BOMBMANAGER_HPP

#include <BomberBlob/Bomb.hpp>
#include <BomberBlob/Explosion.hpp>

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

    static Blob::GL::Texture bombTexture, explostionTexture;

public:
	explicit BombManager(Blob::Vec2f pos, Player &player);

	Bomb *getBomb() const;

	bool update();

	static void initTexture();

	~BombManager();
};

#endif //BOMBERMAN_BOMBMANAGER_HPP
