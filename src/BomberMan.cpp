//
// Created by alexa on 02/03/2018.
//

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <list>
#include <iostream>

#include "BlobEngine/CollisionClass.hpp"

#include "BomberMan.hpp"

using  namespace sf;
using  namespace BlobEngine;

enum directions{
	UP = 0,
	DOWN = 1,
	LEFT = 2,
	RIGHT = 3
};

enum objectTypeEnum{
	EXPLOSION = 0,
	BOMB = 1,
	PLAYER = 2,
	INDESTRUCTIBLE_BOX = 3,
	BOX = 4
};

class Explosion : CircleDynamic{
private:
	CircleShape shape;
	RectangleShape rectShape;
	Clock clock;
	Point2f positionInitial, positionFinal;
	float maxSpeed = 500;
	bool arrived = false;

	Reaction hit(const BlobEngine::PhysicalObject& from) override {
		if(from.objectType == INDESTRUCTIBLE_BOX || from.objectType == BOX){
			arrived = true;
			return STOP;
		}
		return IGNORE;
	}

	bool update() {
		if(!arrived) {
			if(Line(positionInitial, mainCircle.position).Length2() > Line(positionInitial, positionFinal).Length2()) {
				arrived = true;
				speed.reset();
			}
		}

		return clock.getElapsedTime().asSeconds() > 0.75f;
	}

public:
	explicit Explosion(Point2f positionInitial, Point2f positionFinal) :
			CircleDynamic(EXPLOSION),
			positionInitial(positionInitial),
			positionFinal(positionFinal) {

		speed = Vec2f(positionInitial, positionFinal);

		mainCircle.position = positionInitial;
		mainCircle.rayon = 4;

		shape.setRadius(mainCircle.rayon);
		shape.setOrigin(mainCircle.rayon, mainCircle.rayon);
		shape.setPosition(Vector2f(mainCircle.position.x, mainCircle.position.y));
		shape.setFillColor(Color::Blue);

		rectShape.setFillColor(Color::Blue);
		rectShape.setSize(Vector2f(8, 8));
		rectShape.setPosition(Vector2f(mainCircle.position.x, mainCircle.position.y));
		rectShape.setOrigin(Vector2f(4, 4));
		rectShape.setRotation(speed.getOrientationDeg());

		speed.setLength(maxSpeed);
	}

	bool draw(RenderWindow *window) {
		bool r = update();
		shape.setPosition(Vector2f(mainCircle.position.x, mainCircle.position.y));
		window->draw(shape);

		rectShape.setSize(Vector2f(Line(positionInitial, mainCircle.position).Length() + 4, 8));
		window->draw(rectShape);
		return r;
	}
};

class Bomb : CircleDynamic{
private:
	CircleShape shape;
	sf::Clock clock;
	float timeDelay = 2;
	bool destroyed = false;

	Reaction hit(const BlobEngine::PhysicalObject& from) override {
		if(from.objectType == EXPLOSION)
			destroyed = true;

		return IGNORE;
	}

	bool update(){
		return (clock.getElapsedTime().asSeconds() > timeDelay) || destroyed;
	}
public:
	explicit Bomb(Point2f position) : CircleDynamic(BOMB){
		mainCircle.position.x = position.x;
		mainCircle.position.y = position.y;
		mainCircle.rayon = 8;

		shape.setRadius(mainCircle.rayon);
		shape.setOrigin(8, 8);
		shape.setPosition(Vector2f(mainCircle.position.x, mainCircle.position.y));
		shape.setFillColor(Color::Yellow);

		clock.restart();
	}

	bool draw(RenderWindow *window) {
		bool r = update();
		window->draw(shape);
		return r;
	}

	Point2f getPosition(){
		return mainCircle.position;
	}
};

class BombManager{
private:
	std::list<Bomb>	bombs;
	std::list<Explosion> explosions;

	float explosionLenght = 100;

public:

	void addBomb(Point2f pos){
		bombs.emplace_front(pos);
	}

	void draw(RenderWindow *window) {

		unsigned int bombsToDestroy = 0, explosionsToDestroy = 0;

		auto bombIt = bombs.begin();
		while(bombIt != bombs.end()){
			if(bombIt->draw(window)){
				explosions.emplace_back(bombs.back().getPosition(), bombs.back().getPosition() + Vec2f(explosionLenght, 0));
				explosions.emplace_back(bombs.back().getPosition(), bombs.back().getPosition() + Vec2f(-explosionLenght, 0));
				explosions.emplace_back(bombs.back().getPosition(), bombs.back().getPosition() + Vec2f(0, explosionLenght));
				explosions.emplace_back(bombs.back().getPosition(), bombs.back().getPosition() + Vec2f(0, -explosionLenght));

				bombIt = bombs.erase(bombIt);
			}else
				bombIt++;
		}

		auto explosionIt = explosions.begin();
		while(explosionIt != explosions.end()){
			if(explosionIt->draw(window)){
				explosionIt = explosions.erase(explosionIt);
			}else
				explosionIt++;
		}
	}
};

class IndestructibleBox : LineStatic{
private:
	RectangleShape shape;

public:
	explicit IndestructibleBox(Point2f position) : LineStatic(INDESTRUCTIBLE_BOX){

		shape.setSize(Vector2f(20, 20));
		shape.setOrigin(10, 10);
		shape.setPosition(Vector2f(position.x, position.y));
		shape.setFillColor(Color::Red);

		Point2f a = position + Point2f(10, 10);
		Point2f b = position + Point2f(10, -10);
		Point2f c = position + Point2f(-10, -10);
		Point2f d = position + Point2f(-10, 10);

		lines.emplace_back(a, b);
		lines.emplace_back(b, c);
		lines.emplace_back(c, d);
		lines.emplace_back(d, a);
	}

	void draw(RenderWindow *window) {
		window->draw(shape);
	}
};

class Box : LineStatic{
private:
	RectangleShape shape;
	bool destroyed = false;

	Reaction hit(const BlobEngine::PhysicalObject& from) override {
		if(from.objectType == EXPLOSION)
			destroyed = true;

		return IGNORE;
	}

public:
	explicit Box(Point2f position) : LineStatic(BOX){

		shape.setSize(Vector2f(20, 20));
		shape.setOrigin(10, 10);
		shape.setPosition(Vector2f(position.x, position.y));
		shape.setFillColor(Color(255, 150, 0));

		Point2f a = position + Point2f(10, 10);
		Point2f b = position + Point2f(10, -10);
		Point2f c = position + Point2f(-10, -10);
		Point2f d = position + Point2f(-10, 10);

		lines.emplace_back(a, b);
		lines.emplace_back(b, c);
		lines.emplace_back(c, d);
		lines.emplace_back(d, a);
	}

	bool draw(RenderWindow *window) {
		if(!destroyed)
			window->draw(shape);
		return destroyed;
	}
};

class Player : CircleDynamic{
private:
	float maxSpeed;
	CircleShape shape;
	Clock clock;
	bool alive = true;

	std::array<bool, 4> command = {false, false, false, false};

	BombManager *bombManager;

	Reaction hit(const BlobEngine::PhysicalObject& from) override {
		return ROLL;
	}

	void update() {
		float TimeFlow = clock.restart().asSeconds();
		Vec2f Acceleration;

		if (command[directions::LEFT]) {
			Acceleration.x -= 1;
		}
		if (command[directions::RIGHT]) {
			Acceleration.x += 1;
		}
		if (command[directions::UP]) {
			Acceleration.y -= 1;
		}
		if (command[directions::DOWN]) {
			Acceleration.y += 1;
		}

		if (Acceleration.x != 0 || Acceleration.y != 0) {
			speed = Acceleration.setLength(maxSpeed);
		}
		else {
			speed = Vec2f();
		}
	}
public:

	explicit Player(BombManager *bm) : CircleDynamic(PLAYER){
		bombManager = bm;

		mainCircle.position.x = 30;
		mainCircle.position.y = 30;
		mainCircle.rayon = 10;

		shape.setRadius(mainCircle.rayon);
		shape.setOrigin(10, 10);
		shape.setPosition(Vector2f(mainCircle.position.x, mainCircle.position.y));
		maxSpeed = 200;
	}

	void draw(RenderWindow *window) {
		update();

		shape.setPosition(Vector2f(mainCircle.position.x, mainCircle.position.y));
		window->draw(shape);
	}

	void keyPress(directions d) {
		command[d] = true;
	}

	void keyReleased(directions d) {
		command[d] = false;
	}

	void putBomb() {
		bombManager->addBomb(mainCircle.position);
	}
};

//Game manager
BomberMan::BomberMan() {
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	unsigned int width = 620, height = 620;

	sf::RenderWindow window(sf::VideoMode(width, height), "BomberMan", sf::Style::Default, settings);

	window.setFramerateLimit(60);
	window.setKeyRepeatEnabled(false);

	CollisionDetector collisionDetector;

	BombManager bombManager;

	Player player(&bombManager);

	std::list<IndestructibleBox> indestructibleBoxs;
	std::list<Box> boxs;

	for(int i = 80; i < width - 80; i+=40) {
		boxs.emplace_back(Point2f(10 + i, 10 + 20));
		boxs.emplace_back(Point2f(10 + i, height - 10 - 20));
	}

	for(int i = 80; i < height - 80; i+=40){
		boxs.emplace_back(Point2f(10 + 20, 10 + i));
		boxs.emplace_back(Point2f(width - 10 - 20, 10 + i));
	}

	for(int i = 60; i < width - 60; i+=40){
		for(int j = 40; j < height - 40; j+=40)
			boxs.emplace_back(Point2f(10 + i, 10 + j));
	}

	for(int i = 40; i < width - 40; i+=40){
		for(int j = 60; j < height - 60; j+=40)
			boxs.emplace_back(Point2f(10 + i, 10 + j));
	}

	for(int i = 0; i < width; i+=20){
		indestructibleBoxs.emplace_back(Point2f(10 + i, 10));
	}

	for(int i = 0; i < width; i+=20){
		indestructibleBoxs.emplace_back(Point2f(10 + i, height - 10));
	}

	for(int i = 20; i < height - 20; i+=20){
		indestructibleBoxs.emplace_back(Point2f(10, 10 + i));
	}

	for(int i = 20; i < height - 20; i+=20){
		indestructibleBoxs.emplace_back(Point2f(width - 10, 10 + i));
	}

	for(int i = 40; i < width - 40; i+=40){
		for(int j = 40; j < height - 40; j+=40)
			indestructibleBoxs.emplace_back(Point2f(10 + i, 10 + j));
	}

	while (window.isOpen()) {
		Keyboard::Key Key;
		Event event{};
		while (window.pollEvent(event)) {
			switch(event.type){
				case Event::Closed :
					window.close();
				case Event::KeyPressed :
					Key = event.key.code;

					switch(Key) {
						case Keyboard::Left :
							player.keyPress(directions::LEFT);
							break;
						case Keyboard::Right :
							player.keyPress(directions::RIGHT);
							break;
						case Keyboard::Up :
							player.keyPress(directions::UP);
							break;
						case Keyboard::Down :
							player.keyPress(directions::DOWN);
							break;
						case Keyboard::Space :
							player.putBomb();
							break;
						default:
							break;
					}
					break;
				case Event::KeyReleased :
					Key = event.key.code;

					switch(Key) {
						case Keyboard::Left :
							player.keyReleased(directions::LEFT);
							break;
						case Keyboard::Right :
							player.keyReleased(directions::RIGHT);
							break;
						case Keyboard::Up :
							player.keyReleased(directions::UP);
							break;
						case Keyboard::Down :
							player.keyReleased(directions::DOWN);
							break;
						default:
							break;
					}
					break;
				default:
					break;
			}
		}

		window.clear();

		bombManager.draw(&window);

		player.draw(&window);

		auto indestructibleBoxsIt = indestructibleBoxs.begin();
		while(indestructibleBoxsIt != indestructibleBoxs.end()) {
			indestructibleBoxsIt->draw(&window);
			indestructibleBoxsIt++;
		}

		auto BoxsIt = boxs.begin();
		while(BoxsIt != boxs.end()) {
			if(BoxsIt->draw(&window)) {
				BoxsIt = boxs.erase(BoxsIt);
			} else
				BoxsIt++;
		}

		collisionDetector.update();

		window.display();
	}
}
