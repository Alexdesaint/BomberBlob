#include <BomberBlob/Explosion.hpp>

Explosion::Explosion(Blob::Vec2f positionInitial, Blob::Vec2f dirrection, float distanceMax, Blob::Mesh &mesh, float scale) :
        Blob::Shape(mesh, positionInitial.x, positionInitial.y, scale/2.f, scale/2.f, scale/2.f, scale/2.f),
        RectDynamic(positionInitial, {scale, scale}, EXPLOSION),
        distanceMax(distanceMax),
        positionInitial(positionInitial),
        dirrection(dirrection) {

	dirrection = dirrection.getNormal();
	speed = dirrection * maxSpeed;

    setReaction(IGNORE);
}

void Explosion::postCollisionUpdate() {
    Shape::setPosition(position, 0.4f);
}

void Explosion::hit(int objectType, Object &object) {
    if (objectType == INDESTRUCTIBLE_BOX || objectType == BOX || objectType == BOMB)
		active = false;
}

bool Explosion::keepMoving() {
    if ((positionInitial - position).length2() > distanceMax * distanceMax) {
		active = false;
		return false;
	}

	return true;
}
