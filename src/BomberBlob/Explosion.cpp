#include <BomberBlob/Explosion.hpp>

Explosion::Explosion(Blob::Vec2f positionInitial, Blob::Vec2f dirrection, float distanceMax) :
        RectDynamic(positionInitial, {0.4f, 0.4f}, EXPLOSION),
        distanceMax(distanceMax),
        positionInitial(positionInitial),
        dirrection(dirrection) {

	dirrection = dirrection.getNormal();
	speed = dirrection * maxSpeed;

    Cube::setPosition(positionInitial.x, positionInitial.y, 0.2f);
	setScale(0.4f, 0.4f, 0.4f);

	loadBMP("data/Explosion.bmp");

    setReaction(IGNORE);
}

void Explosion::postCollisionUpdate() {
    Cube::setPosition(getPosition(), 0.4f);
}

void Explosion::hit(int objectType, Object &object) {
    if (objectType == INDESTRUCTIBLE_BOX || objectType == BOX || objectType == BOMB)
		active = false;
}

bool Explosion::keepMoving() {
    if ((positionInitial - getPosition()).length2() > distanceMax * distanceMax) {
		active = false;
		return false;
	}

	return true;
}
