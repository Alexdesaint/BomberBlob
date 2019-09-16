#include <BomberBlob/Textures.hpp>

Textures::Textures() : ground("data/Grass.bmp"),
                       box("data/Box.bmp"),
                       indestructibleBox("data/IndestructibleBox.bmp"),
                       extraBomb("data/ExtraBomb.bmp"),
                       extraPower("data/ExtraPower.bmp"),
                       extraSpeed("data/ExtraSpeed.bmp"),
                       bomb("data/Bomb.bmp"),
                       explosion("data/Explosion.bmp") {
	player[0].setColor(255, 0, 0);
	player[1].setColor(127, 0, 127);
	player[2].setColor(0, 0, 255);
	player[3].setColor(255, 255, 0);
}
