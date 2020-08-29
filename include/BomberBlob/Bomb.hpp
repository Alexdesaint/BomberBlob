#pragma once

#include <Blob/Collision/CollisionDetector.hpp>
#include <Blob/Shape.hpp>

#include <BomberBlob/UserData.hpp>

class Player;

class Bomb : public Blob::Collision::RectStatic, public Blob::Shape {
private:
	bool destroyed = false;

public:
	explicit Bomb(Blob::Vec2f pos, Blob::Mesh &mesh, float scale = 0.4f);

    void hit(int objectType, Object &object) final;

	bool isDestroyed();
};
