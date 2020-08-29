#pragma once

#include <Blob/Collision/CollisionDetector.hpp>
#include <Blob/Shape.hpp>
#include <Blob/Time.hpp>

#include <BomberBlob/UserData.hpp>

class Explosion : public Blob::Collision::RectDynamic, public Blob::Shape {
private:
	Blob::Vec2f positionInitial, dirrection;
	float maxSpeed = 8, distanceMax = 2;
	bool active = true;

public:
	Explosion(Blob::Vec2f positionInitial, Blob::Vec2f dirrection, float distanceMax, Blob::Mesh &mesh, float scale = 0.4f);

	bool keepMoving() final;

	void postCollisionUpdate() final;

    void hit(int objectType, Object &object) final;

	bool isActive() const {
		return active;
	}

};
