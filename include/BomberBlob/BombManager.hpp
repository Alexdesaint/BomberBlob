#pragma once

#include <BomberBlob/Bomb.hpp>
#include <BomberBlob/Explosion.hpp>
#include <BomberBlob/Textures.hpp>

#include <Blob/GL/Shape.hpp>

#include <Blob/Time.hpp>

class Bomber;

class BombManager : public Blob::GL::Shape {
private:
	Bomb *bomb;
	float bombDelay = 3;

	Explosion *exUP, *exDO, *exRI, *exLE;

	Blob::Time::TimePoint start;

	Bomber &bomber;

    Textures &textures;

public:
	explicit BombManager(Blob::Vec2f pos, Bomber &bomber, Textures &textures);

    ~BombManager();

	Bomb *getBomb() const;

	bool update();
};
