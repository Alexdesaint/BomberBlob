#include <BomberBlob/IndestructibleBox.hpp>

IndestructibleBox::IndestructibleBox(float x, float y, float scale) : RectStatic({x, y}, {scale, scale}, INDESTRUCTIBLE_BOX) {
    Cube::setPosition(x, y, scale/2.f);
    Cube::setScale(scale);

	loadBMP("data/IndestructibleBox.bmp");
	setTextureScale({4.f, 4.f});
}
