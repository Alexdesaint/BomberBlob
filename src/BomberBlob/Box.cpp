#include <BomberBlob/Box.hpp>

Box::Box(float x, float y, Blob::Mesh &mesh, float scale) : RectStatic({x, y}, {scale, scale}, BOX),
                                               Blob::Shape(mesh, x, y, scale/2.f, scale/2.f, scale/2.f, scale/2.f) {
    //setTexture(texture);
    //setTextureScale({4.f, 4.f});
}

void Box::hit(int objectType, Object &object) {
    if (objectType == EXPLOSION)
        destroyed = true;
}
