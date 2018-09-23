#include <BomberBlob/Bomb.hpp>

using namespace sf;

bool Bomb::update() {
	return (clock.getElapsedTime().asSeconds() > timeDelay) || destroyed;
}

Bomb::Bomb(b2Vec2 p, b2World *world) : StaticCircle( p.x, p.y, 8, &userData, world){
	position = p;

	shape.setRadius(8);
	shape.setOrigin(8, 8);
	shape.setPosition(sf::Vector2f(position.x, position.y));
	shape.setFillColor(Color::Yellow);

	clock.restart();
}

bool Bomb::draw(RenderWindow *window) {
	if(!bombAlone && !playerOnBomb)
		bombAlone = true;
	playerOnBomb = false;
	bool r = update();
	window->draw(shape);
	return r;
}
