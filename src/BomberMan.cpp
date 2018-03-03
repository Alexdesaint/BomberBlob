//
// Created by alexa on 02/03/2018.
//

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <list>

#include "BlobEngine/CollisionClass.hpp"

#include "BomberMan.hpp"


using  namespace sf;

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
	INDESCTRUCTIBLEBOX,
	BOX
};

class Explosion : Physics::CircleDynamic{
private:

	CircleShape shape;
	RectangleShape rectShape;
	Clock clock;
	Geometrie::Point2f positionInitial;
	float mexLength, totalTimeFlow = 0;

	bool update(){
		float TimeFlow = clock.restart().asSeconds();
		totalTimeFlow += TimeFlow;

		Geometrie::Vec2f v(speed * TimeFlow);

		if(CheckCollision(&v)){
			speed.x = 0;
			speed.y = 0;
		}

		mainCircle.position = mainCircle.position + v;

		shape.setPosition(Vector2f(mainCircle.position.x, mainCircle.position.y));

		float length = Geometrie::Vec2f(positionInitial, mainCircle.position).Length();

		rectShape.setSize(Vector2f(length + 4, 8));

		if(length > mexLength){
			speed.x = 0;
			speed.y = 0;
		}

		return totalTimeFlow > 0.75f;
	}

public:
	explicit Explosion(Physics::CollisionDetector *collisionDetector1, Geometrie::Point2f positionInitial, Geometrie::Vec2f speed, float mexLength) :
			CircleDynamic(collisionDetector1, EXPLOSION),
			mexLength(mexLength),
			positionInitial(positionInitial) {

		this->speed = speed;

		mainCircle.position = positionInitial;
		mainCircle.rayon = 4;

		shape.setRadius(mainCircle.rayon);
		shape.setOrigin(4, 4);
		shape.setPosition(Vector2f(mainCircle.position.x, mainCircle.position.y));
		shape.setFillColor(Color::Blue);

		rectShape.setFillColor(Color::Blue);
		rectShape.setSize(Vector2f(8, 8));
		rectShape.setPosition(Vector2f(mainCircle.position.x, mainCircle.position.y));
		rectShape.setOrigin(Vector2f(4, 4));
		rectShape.setRotation(static_cast<float>(std::atan2(speed.x, speed.y) * -180 / PI) + 90);
	}

	bool draw(RenderWindow *window) {
		bool r = update();
		window->draw(shape);
		window->draw(rectShape);
		return r;
	}
};

class Bomb : Physics::CircleDynamic{
private:
	CircleShape shape;
	sf::Clock clock;
	float timeDelay = 2;
	bool destroyed = false;

	void hit(const Physics::PhysicalObject& from) override {
		if(from.objectType == EXPLOSION)
			destroyed = true;
	}

	bool update(){
		return (clock.getElapsedTime().asSeconds() > timeDelay) || destroyed;
	}
public:
	explicit Bomb(Physics::CollisionDetector *collisionDetector1, Geometrie::Point2f position) : CircleDynamic(collisionDetector1, BOMB){
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

	Geometrie::Point2f getPosition(){
		return mainCircle.position;
	}
};

class BombManager{
private:
	std::list<Bomb>	bombs;
	std::list<Explosion> explosions;
	Physics::CollisionDetector *collisionDetector;
	float explosionSpeed = 500;

public:
	explicit BombManager(Physics::CollisionDetector *c){
		collisionDetector = c;
	}

	void addBomb(Geometrie::Point2f pos){
		bombs.emplace_front(collisionDetector, pos);
	}

	void draw(RenderWindow *window) {

		unsigned int bombsToDestroy = 0, explosionsToDestroy = 0;

		auto bombIt = bombs.begin();
		while(bombIt != bombs.end()){
			if(bombIt->draw(window)){
				explosions.emplace_back(collisionDetector, bombs.back().getPosition(), Geometrie::Vec2f(explosionSpeed, 0), 100);
				explosions.emplace_back(collisionDetector, bombs.back().getPosition(), Geometrie::Vec2f(-explosionSpeed, 0), 100);
				explosions.emplace_back(collisionDetector, bombs.back().getPosition(), Geometrie::Vec2f(0, explosionSpeed), 100);
				explosions.emplace_back(collisionDetector, bombs.back().getPosition(), Geometrie::Vec2f(0, -explosionSpeed), 100);

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

class IndestructibleBox : Physics::LineStatic{
private:
	RectangleShape shape;

public:
	explicit IndestructibleBox(Physics::CollisionDetector *collisionDetector, Geometrie::Point2f position) : LineStatic(collisionDetector, INDESCTRUCTIBLEBOX){

		shape.setSize(Vector2f(20, 20));
		shape.setOrigin(10, 10);
		shape.setPosition(Vector2f(position.x, position.y));
		shape.setFillColor(Color::Red);

		Geometrie::Point2f a = position + Geometrie::Point2f(10, 10);
		Geometrie::Point2f b = position + Geometrie::Point2f(10, -10);
		Geometrie::Point2f c = position + Geometrie::Point2f(-10, -10);
		Geometrie::Point2f d = position + Geometrie::Point2f(-10, 10);

		lines.emplace_back(a, b);
		lines.emplace_back(b, c);
		lines.emplace_back(c, d);
		lines.emplace_back(d, a);
	}

	void draw(RenderWindow *window) {
		window->draw(shape);
	}
};

class Box : Physics::LineStatic{
private:
	RectangleShape shape;
	bool destroyed = false;

	void hit(const Physics::PhysicalObject& from) override {
		if(from.objectType == EXPLOSION)
			destroyed = true;
	}

public:
	explicit Box(Physics::CollisionDetector *collisionDetector, Geometrie::Point2f position) : LineStatic(collisionDetector, BOX){

		shape.setSize(Vector2f(20, 20));
		shape.setOrigin(10, 10);
		shape.setPosition(Vector2f(position.x, position.y));
		shape.setFillColor(Color(255, 150, 0));

		Geometrie::Point2f a = position + Geometrie::Point2f(10, 10);
		Geometrie::Point2f b = position + Geometrie::Point2f(10, -10);
		Geometrie::Point2f c = position + Geometrie::Point2f(-10, -10);
		Geometrie::Point2f d = position + Geometrie::Point2f(-10, 10);

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

class Player : Physics::CircleDynamic{
private:
	float maxSpeed, orientation;
	CircleShape shape;
	Clock clock;
	bool alive = true;

	std::array<bool, 4> command = {false, false, false, false};

	BombManager *bombManager;

	void update() {
		float TimeFlow = clock.restart().asSeconds();
		Geometrie::Vec2f Acceleration;

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

		Geometrie::Vec2f ActualSpeed;

		if (Acceleration.x != 0 || Acceleration.y != 0) {
			orientation = std::atan2(Acceleration.y, Acceleration.x);
			speed = Acceleration.setLength(maxSpeed);
			ActualSpeed = speed * TimeFlow;
			CheckCollision(&ActualSpeed);
		}
		else {
			speed = Geometrie::Vec2f();
		}

		mainCircle.position = mainCircle.position + ActualSpeed;

		shape.setPosition(Vector2f(mainCircle.position.x, mainCircle.position.y));
	}
public:

	explicit Player(Physics::CollisionDetector *collisionDetector, BombManager *bm) : CircleDynamic(collisionDetector, PLAYER){
		bombManager = bm;

		mainCircle.position.x = 30;
		mainCircle.position.y = 30;
		mainCircle.rayon = 10;
		orientation = 0;

		shape.setRadius(mainCircle.rayon);
		shape.setOrigin(10, 10);
		shape.setPosition(Vector2f(mainCircle.position.x, mainCircle.position.y));
		maxSpeed = 200;
	}

	void draw(RenderWindow *window) {
		update();
		window->draw(shape);
	}

	void keyPress(directions d) {
		command[d] = true;
	}

	void keyReleased(directions d) {
		command[d] = false;
	}

	void putBomb(Physics::CollisionDetector *collisionDetector) {
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

	Physics::CollisionDetector collisionDetector;

	BombManager bombManager(&collisionDetector);

	Player player(&collisionDetector, &bombManager);

	std::list<IndestructibleBox> indestructibleBoxs;
	std::list<Box> boxs;

	for(int i = 80; i < width - 80; i+=40) {
		boxs.emplace_back(&collisionDetector, Geometrie::Point2f(10 + i, 10 + 20));
		boxs.emplace_back(&collisionDetector, Geometrie::Point2f(10 + i, height - 10 - 20));
	}

	for(int i = 80; i < height - 80; i+=40){
		boxs.emplace_back(&collisionDetector, Geometrie::Point2f(10 + 20, 10 + i));
		boxs.emplace_back(&collisionDetector, Geometrie::Point2f(width - 10 - 20, 10 + i));
	}

	for(int i = 60; i < width - 60; i+=40){
		for(int j = 40; j < height - 40; j+=40)
			boxs.emplace_back(&collisionDetector, Geometrie::Point2f(10 + i, 10 + j));
	}

	for(int i = 40; i < width - 40; i+=40){
		for(int j = 60; j < height - 60; j+=40)
			boxs.emplace_back(&collisionDetector, Geometrie::Point2f(10 + i, 10 + j));
	}

	for(int i = 0; i < width; i+=20){
		indestructibleBoxs.emplace_back(&collisionDetector, Geometrie::Point2f(10 + i, 10));
	}

	for(int i = 0; i < width; i+=20){
		indestructibleBoxs.emplace_back(&collisionDetector, Geometrie::Point2f(10 + i, height - 10));
	}

	for(int i = 20; i < height - 20; i+=20){
		indestructibleBoxs.emplace_back(&collisionDetector, Geometrie::Point2f(10, 10 + i));
	}

	for(int i = 20; i < height - 20; i+=20){
		indestructibleBoxs.emplace_back(&collisionDetector, Geometrie::Point2f(width - 10, 10 + i));
	}

	for(int i = 40; i < width - 40; i+=40){
		for(int j = 40; j < height - 40; j+=40)
			indestructibleBoxs.emplace_back(&collisionDetector, Geometrie::Point2f(10 + i, 10 + j));
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
							player.putBomb(&collisionDetector);
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

		for(IndestructibleBox ib : indestructibleBoxs)
			ib.draw(&window);

		auto BoxsIt = boxs.begin();
		while(BoxsIt != boxs.end()){
			if(BoxsIt->draw(&window)){
				BoxsIt = boxs.erase(BoxsIt);
			}else
				BoxsIt++;
		}

		window.display();
	}
}
