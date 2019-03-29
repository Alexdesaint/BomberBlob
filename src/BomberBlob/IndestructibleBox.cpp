#include <BomberBlob/IndestructibleBox.hpp>

IndestructibleBox::IndestructibleBox(float x, float y) : RectStatic({x, y}, {1, 1}, INDESTRUCTIBLE_BOX) {
    Cube::setPosition(x, y, 0.5f);

	loadBMP("data/IndestructibleBox.bmp");
	setTextureScale({4.f, 4.f});
}
