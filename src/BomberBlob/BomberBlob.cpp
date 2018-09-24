#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <cmath>
#include <list>
#include <iostream>

#include <BomberBlob/BomberBlob.hpp>

#include <BomberBlob/ContactListener.hpp>
#include <BomberBlob/BombManager.hpp>
#include <BomberBlob/Player.hpp>
#include <BomberBlob/IndestructibleBox.hpp>
#include <BomberBlob/Box.hpp>

using  namespace sf;

BomberBlob::BomberBlob(sf::RenderWindow &window) {
	unsigned int width = 620, height = 620;

	b2Vec2 gravity(0.0f, 0.0f);
	b2World world(gravity);

	ContactListener contactListener;
	world.SetContactListener(&contactListener);

	BombManager bombManager(&world);

	Player player(&bombManager, world);
	
	std::list<IndestructibleBox> indestructibleBoxs;
	std::list<Box> boxs;

	for(int i = 80; i < width - 80; i+=40) {
		boxs.emplace_back(10 + i, 10 + 20, world);
		boxs.emplace_back(10 + i, height - 10 - 20, world);
	}

	for(int i = 80; i < height - 80; i+=40){
		boxs.emplace_back(10 + 20, 10 + i, world);
		boxs.emplace_back(width - 10 - 20, 10 + i, world);
	}

	for(int i = 60; i < width - 60; i+=40){
		for(int j = 40; j < height - 40; j+=40) {
			boxs.emplace_back(10 + i, 10 + j, world);
		}
	}

	for(int i = 40; i < width - 40; i+=40){
		for(int j = 60; j < height - 60; j+=40) {
			boxs.emplace_back(10 + i, 10 + j, world);
		}
	}

	for(int i = 0; i < width; i+=20){
		indestructibleBoxs.emplace_back(10 + i, 10, world);
	}

	for(int i = 0; i < width; i+=20){
		indestructibleBoxs.emplace_back(10 + i, height - 10, world);
	}

	for(int i = 20; i < height - 20; i+=20){
		indestructibleBoxs.emplace_back(10, 10 + i, world);
	}

	for(int i = 20; i < height - 20; i+=20){
		indestructibleBoxs.emplace_back(width - 10, 10 + i, world);
	}

	for(int i = 40; i < width - 40; i+=40){
		for(int j = 40; j < height - 40; j+=40) {
			indestructibleBoxs.emplace_back(10 + i, 10 + j, world);
		}
	}

	bool gameIsRunning = true;

	while (window.isOpen() && gameIsRunning) {
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

		world.Step(1.f/60.f, 8, 3);
		window.display();

		if(!player.isAlive())
			gameIsRunning = false;
	}
}