#pragma once

#include <Blob/Shapes.hpp>
#include <Blob/Materials.hpp>
#include <Core/StaticCollider.hpp>

class Tree : public Blob::Shape, public StaticCollider {
private:
    Blob::Materials::PBRSingleColor trunkMat, leavesMat;
    Blob::Shapes::Cube trunk, leaves;
public:
    Tree(b2World &world, const Blob::Vec3<float> &pos);
};