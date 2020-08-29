#pragma once

#include <Blob/Collision/CollisionDetector.hpp>
#include <Blob/Shape.hpp>
#include <Blob/Time.hpp>

#include <BomberBlob/UserData.hpp>

#include <BomberBlob/Textures.hpp>

class Bonus : public Blob::Collision::RectStatic, public Blob::Shape {
private:
	bool destroyed = false;
public:
	explicit Bonus(Blob::Vec2f pos, Textures &textures, float scale = 0.4f);

    void hit(int objectType, Object &object) final;

	bool update();
};
