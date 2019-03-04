#ifndef BOMBERMAN_BOX_HPP
#define BOMBERMAN_BOX_HPP

#include <Blob/Collision/CollisionDetector.hpp>
#include <Blob/GL/Shapes.hpp>

#include <BomberBlob/UserData.hpp>

class Box : public Blob::Collision::RectStatic, public Blob::GL::Shapes::Cube {
private:
	bool destroyed = false;

public:
	explicit Box(float x, float y);

	void hit(int objectType, const void *objectData) final;

	bool isDestroy() const {
		return destroyed;
	}
};


#endif //BOMBERMAN_BOX_HPP
