#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <list>

#include "CollisionClass.hpp"

using  namespace sf;

enum directions{
	UP = 0,
	DOWN = 1,
	LEFT = 2,
	RIGHT = 3
};

class Bomb : Physics::CircleDynamic{
private:
	CircleShape shape;
	sf::Clock clock;
	float timeDelay = 2;

	bool update(){
		if(clock.getElapsedTime().asSeconds() > timeDelay)
			return true;
		return false;
	}
public:
	explicit Bomb(Physics::CollisionDetector *collisionDetector1, Geometrie::point2f position) : CircleDynamic(collisionDetector1){
		mainCircle.Position.x = position.x;
		mainCircle.Position.y = position.y;
		mainCircle.Rayon = 8;

		shape.setRadius(mainCircle.Rayon);
		shape.setOrigin(8, 8);
		shape.setPosition(Vector2f(mainCircle.Position.x, mainCircle.Position.y));
		shape.setFillColor(Color::Yellow);

		clock.restart();
	}

	bool draw(RenderWindow *window) {
		bool r = update();
		window->draw(shape);
		return r;
	}
};

class BombManager{
private:
	std::list<Bomb>	bombs;
	Physics::CollisionDetector *collisionDetector;
public:
	explicit BombManager(Physics::CollisionDetector *c){
		collisionDetector = c;
	}

	void addBomb(Geometrie::point2f pos){
		bombs.emplace_front(collisionDetector, pos);
	}

	void draw(RenderWindow *window) {

		unsigned int toDestroy = 0;

		for(Bomb bomb : bombs){
			if(bomb.draw(window)){
				toDestroy++;
			}
		}

		for(unsigned int i = 0; i < toDestroy; i++)
			bombs.pop_back();
	}
};

class IndestructibleBox : Physics::LineStatic{
private:
	RectangleShape shape;

public:
	explicit IndestructibleBox(Physics::CollisionDetector *collisionDetector, Geometrie::point2f position) : LineStatic(collisionDetector){

		shape.setSize(Vector2f(20, 20));
		shape.setOrigin(10, 10);
		shape.setPosition(Vector2f(position.x, position.y));
		shape.setFillColor(Color::Red);

		Geometrie::point2f a = position + Geometrie::point2f(10, 10);
		Geometrie::point2f b = position + Geometrie::point2f(10, -10);
		Geometrie::point2f c = position + Geometrie::point2f(-10, -10);
		Geometrie::point2f d = position + Geometrie::point2f(-10, 10);

		lines.emplace_back(a, b);
		lines.emplace_back(b, c);
		lines.emplace_back(c, d);
		lines.emplace_back(d, a);
	}

	void draw(RenderWindow *window) {
		window->draw(shape);
	}
};

class BomberMan : Physics::CircleDynamic{
private:
	float maxSpeed, orientation;
	CircleShape shape;
	Clock clock;

	std::array<bool, 4> command = {false, false, false, false};

	BombManager *bombManager;

	void update() {
		float TimeFlow = clock.restart().asSeconds();
		Geometrie::vec2f Acceleration;

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

		Geometrie::vec2f ActualSpeed;

		if (Acceleration.x != 0 || Acceleration.y != 0) {
			orientation = std::atan2(Acceleration.y, Acceleration.x);
			speed = Acceleration.setLength(maxSpeed);
			ActualSpeed = speed * TimeFlow;
			Checkcollision(&ActualSpeed);
		}
		else {
			speed = Geometrie::vec2f();
		}

		mainCircle.Position = mainCircle.Position + ActualSpeed;

		shape.setPosition(Vector2f(mainCircle.Position.x, mainCircle.Position.y));
	}
public:

	explicit BomberMan(Physics::CollisionDetector *collisionDetector, BombManager *bm) : CircleDynamic(collisionDetector){
		bombManager = bm;

		mainCircle.Position.x = 30;
		mainCircle.Position.y = 30;
		mainCircle.Rayon = 10;
		orientation = 0;

		shape.setRadius(mainCircle.Rayon);
		shape.setOrigin(10, 10);
		shape.setPosition(Vector2f(mainCircle.Position.x, mainCircle.Position.y));
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
		bombManager->addBomb(mainCircle.Position);
	}
};

int main() {
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	unsigned int width = 620, height = 620;

	sf::RenderWindow window(sf::VideoMode(width, height), "BomberMan", sf::Style::Default, settings);

	window.setFramerateLimit(60);
	window.setKeyRepeatEnabled(false);

	Physics::CollisionDetector collisionDetector;

	BombManager bombManager(&collisionDetector);

	BomberMan bomberMan(&collisionDetector, &bombManager);

	std::list<IndestructibleBox> indestructibleBoxs;

	for(int i = 0; i < width; i+=20){
		indestructibleBoxs.emplace_back(&collisionDetector, Geometrie::point2f(10 + i, 10));
	}

	for(int i = 0; i < width; i+=20){
		indestructibleBoxs.emplace_back(&collisionDetector, Geometrie::point2f(10 + i, height - 10));
	}

	for(int i = 20; i < height - 20; i+=20){
		indestructibleBoxs.emplace_back(&collisionDetector, Geometrie::point2f(10, 10 + i));
	}

	for(int i = 20; i < height - 20; i+=20){
		indestructibleBoxs.emplace_back(&collisionDetector, Geometrie::point2f(width - 10, 10 + i));
	}

	for(int i = 40; i < width - 40; i+=40){
		for(int j = 40; j < height - 40; j+=40)
			indestructibleBoxs.emplace_back(&collisionDetector, Geometrie::point2f(10 + i, 10 + j));
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
							bomberMan.keyPress(directions::LEFT);
							break;
						case Keyboard::Right :
							bomberMan.keyPress(directions::RIGHT);
							break;
						case Keyboard::Up :
							bomberMan.keyPress(directions::UP);
							break;
						case Keyboard::Down :
							bomberMan.keyPress(directions::DOWN);
							break;
						case Keyboard::Space :
							bomberMan.putBomb(&collisionDetector);
						default:
							break;
					}
					break;
				case Event::KeyReleased :
					Key = event.key.code;

					switch(Key) {
						case Keyboard::Left :
							bomberMan.keyReleased(directions::LEFT);
							break;
						case Keyboard::Right :
							bomberMan.keyReleased(directions::RIGHT);
							break;
						case Keyboard::Up :
							bomberMan.keyReleased(directions::UP);
							break;
						case Keyboard::Down :
							bomberMan.keyReleased(directions::DOWN);
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

		bomberMan.draw(&window);

		for(IndestructibleBox ib : indestructibleBoxs)
			ib.draw(&window);

		window.display();
	}
	return EXIT_SUCCESS;
}