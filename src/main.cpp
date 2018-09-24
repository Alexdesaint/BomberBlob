#include <SFML/Graphics.hpp>

#include "GameMenu.hpp"

int main() {
	sf::ContextSettings settings;
	settings.antialiasingLevel = 0;

	unsigned int width = 620+100, height = 620;

	sf::RenderWindow window(sf::VideoMode(width, height), "BomberMan", sf::Style::Default, settings);

	window.setFramerateLimit(60);
	window.setKeyRepeatEnabled(false);

	GameMenu gameMenu(window);

	return 0;
}