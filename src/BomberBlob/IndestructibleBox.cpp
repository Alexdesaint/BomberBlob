#include <BomberBlob/IndestructibleBox.hpp>
#include <BomberBlob/UserData.hpp>

IndestructibleBox::IndestructibleBox(const Blob::Vec2<float> &pos, Blob::Material &material, b2World &world, float width)
    : StaticCube(world, pos, width, material, UserData::INDESTRUCTIBLE_BOX) {}
