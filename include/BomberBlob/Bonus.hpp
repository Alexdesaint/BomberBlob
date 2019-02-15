#ifndef PROJECT_EXTRABOMB_HPP
#define PROJECT_EXTRABOMB_HPP

#include <BlobEngine/Collision/CollisionDetector.hpp>
#include <BlobEngine/BlobGL/Shapes.hpp>
#include <BlobEngine/Time.hpp>

#include <BomberBlob/UserData.hpp>

class Bonus : public Blob::Collision::RectStatic, public Blob::GL::Shapes::Cube {
private:
	bool destroyed = false;

public:
	explicit Bonus(Blob::Vec2f pos);

	void hit(int objectType, const void *objectData) final;

	bool update();
};


#endif //PROJECT_EXTRABOMB_HPP
