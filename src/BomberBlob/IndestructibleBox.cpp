#include <BomberBlob/IndestructibleBox.hpp>

IndestructibleBox::IndestructibleBox(float x, float y, Blob::Mesh &mesh, float scale) :
        Blob::Shape(mesh, x, y, scale / 2.f, scale/2.f, scale/2.f, scale/2.f),
        RectStatic({x, y}, {scale, scale}, INDESTRUCTIBLE_BOX) {}
