#pragma once

#include <Blob/Collision/CollisionDetector.hpp>
#include <Blob/Shape.hpp>

#include <BomberBlob/UserData.hpp>

class Box : public Blob::Collision::RectStatic, public Blob::Shape {
private:
	bool destroyed = false;

public:
	explicit Box(float x, float y, Blob::Mesh &mesh, float scale = 0.8f);

    void hit(int objectType, Object &object) final;

	bool isDestroy() const {
		return destroyed;
	}
};
