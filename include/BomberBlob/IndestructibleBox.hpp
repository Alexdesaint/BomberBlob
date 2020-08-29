#pragma once

#include <Blob/Collision/CollisionDetector.hpp>
#include <Blob/Shape.hpp>

#include <BomberBlob/UserData.hpp>

class IndestructibleBox : public Blob::Collision::RectStatic, public Blob::Shape {
private:
	bool destroyed = false;

public:
	IndestructibleBox(float x, float y, Blob::Mesh &mesh, float scale = 1.f);
};
