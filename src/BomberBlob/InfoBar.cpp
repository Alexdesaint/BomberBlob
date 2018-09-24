#include <BomberBlob/InfoBar.hpp>

using namespace sf;

sf::Text InfoBar::createText(const sf::String &str, unsigned int size, int x, int y) {
	sf::Text text(str, font);
	text.setCharacterSize(size);
	//text.setStyle(sf::Text::Bold);
	text.setFillColor(sf::Color::White);
	sf::FloatRect textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f);
	text.setPosition(x, y);

	Image image;
	image.loadFromFile("../data/ExtraPower.bmp");
	image.createMaskFromColor(sf::Color(0, 255, 0), 0);
	ExtraPower.loadFromImage(image);

	image.loadFromFile("../data/ExtraLife.bmp");
	image.createMaskFromColor(sf::Color(0, 255, 0), 0);
	ExtraLife.loadFromImage(image);

	image.loadFromFile("../data/ExtraBomb.bmp");
	image.createMaskFromColor(sf::Color(0, 255, 0), 0);
	ExtraBomb.loadFromImage(image);

	return text;
}

InfoBar::InfoBar() {
	font.loadFromFile("../Data/FFFFORWA.TTF");

	shape.setSize(sf::Vector2f(100, 620));
	shape.setPosition(sf::Vector2f(620, 0));
	shape.setFillColor(sf::Color(30, 30, 30));
}

void InfoBar::draw(sf::RenderWindow *window) {
	window->draw(shape);

	window->draw(createText("Player 1", 12, 670, 15));
	window->draw(createText("Player 2", 12, 670, 15 + 155));
	window->draw(createText("Player 3", 12, 670, 15 + 155 * 2));
	window->draw(createText("Player 4", 12, 670, 15 + 155 * 3));

	RectangleShape rec;
	rec.setSize({16, 16});
	rec.setOrigin(8,8);
	rec.setTexture(&ExtraLife);

	rec.setPosition(650, 40);
	window->draw(rec);
	window->draw(createText("1", 12, 690, 40));

	rec.setPosition(650, 40 + 155);
	window->draw(rec);
	window->draw(createText("0", 12, 690, 40 + 155));

	rec.setPosition(650, 40 + 155 * 2);
	window->draw(rec);
	window->draw(createText("0", 12, 690, 40 + 155 * 2));

	rec.setPosition(650, 40 + 155 * 3);
	window->draw(rec);
	window->draw(createText("0", 12, 690, 40 + 155 * 3));


	rec.setTexture(&ExtraBomb);

	rec.setPosition(650, 70);
	window->draw(rec);
	window->draw(createText("0", 12, 690, 70));

	rec.setPosition(650, 70 + 155);
	window->draw(rec);
	window->draw(createText("0", 12, 690, 70 + 155));

	rec.setPosition(650, 70 + 155 * 2);
	window->draw(rec);
	window->draw(createText("0", 12, 690, 70 + 155 * 2));

	rec.setPosition(650, 70 + 155 * 3);
	window->draw(rec);
	window->draw(createText("0", 12, 690, 70 + 155 * 3));

	rec.setTexture(&ExtraPower);

	rec.setPosition(650, 100);
	window->draw(rec);
	window->draw(createText("0", 12, 690, 100));

	rec.setPosition(650, 100 + 155);
	window->draw(rec);
	window->draw(createText("0", 12, 690, 100 + 155));

	rec.setPosition(650, 100 + 155 * 2);
	window->draw(rec);
	window->draw(createText("0", 12, 690, 100 + 155 * 2));

	rec.setPosition(650, 100 + 155 * 3);
	window->draw(rec);
	window->draw(createText("0", 12, 690, 100 + 155 * 3));
}
