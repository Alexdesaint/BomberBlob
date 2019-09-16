#pragma once

#include <Blob/Collision/CollisionDetector.hpp>
#include <Blob/GL/Shapes.hpp>

#include <BomberBlob/UserData.hpp>

class IndestructibleBox : public Blob::Collision::RectStatic, public Blob::GL::Shapes::Cube {
private:
	bool destroyed = false;

public:
	IndestructibleBox(float x, float y, Blob::GL::Texture &texture, float scale = 1.f);
};
