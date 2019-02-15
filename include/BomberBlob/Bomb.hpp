#ifndef BOMBERMAN_BOMB_HPP
#define BOMBERMAN_BOMB_HPP

#include <BlobEngine/Collision/CollisionDetector.hpp>
#include <BlobEngine/BlobGL/Shapes.hpp>

#include <BomberBlob/UserData.hpp>

class Player;

class Bomb : public Blob::Collision::RectStatic, public Blob::GL::Shapes::Cube {
private:

	bool destroyed = false;

public:
	explicit Bomb(Blob::Vec2f pos);

	void hit(int objectType, const void *objectData) final;

	bool isDestroyed();
};

#endif //BOMBERMAN_BOMB_HPP
