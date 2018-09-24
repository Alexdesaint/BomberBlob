#ifndef BOMBERMAN_INFOBAR_HPP
#define BOMBERMAN_INFOBAR_HPP

#include <SFML/Graphics.hpp>

class InfoBar {
private:
	sf::RectangleShape shape;
	sf::Font font;

	sf::Texture ExtraPower;
	sf::Texture ExtraLife;
	sf::Texture ExtraBomb;

	sf::Text createText(const sf::String &str, unsigned int size, int x , int y);

public:
	InfoBar();

	void draw(sf::RenderWindow *window);
};

#endif //BOMBERMAN_INFOBAR_HPP
