#ifndef BOMBERMAN_INDESTRUCTIBLEBOX_HPP
#define BOMBERMAN_INDESTRUCTIBLEBOX_HPP

#include <BlobEngine/Collision/CollisionDetector.hpp>
#include <BlobEngine/BlobGL/Shapes.hpp>

#include <BomberBlob/UserData.hpp>

class IndestructibleBox : public Blob::Collision::RectStatic, public Blob::GL::Shapes::Cube {
private:
	bool destroyed = false;

public:
	IndestructibleBox(float x, float y);
};

#endif //BOMBERMAN_INDESTRUCTIBLEBOX_HPP
