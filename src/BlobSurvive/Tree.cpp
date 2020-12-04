#include <BlobSurvive/Tree.hpp>

#include <Blob/Color.hpp>

using namespace Blob;

Tree::Tree(b2World &world, const Blob::Maths::Vec3<float> &pos)
    : StaticCollider(world, {pos.x, pos.y}, {0.4f, 0.4f}), trunkMat(Color::Brown), leavesMat(Color::ForestGreen), trunk(trunkMat), leaves(leavesMat) {
    addChild(trunk);
    addChild(leaves);
    setPosition(pos);
    trunk.setPosition({0,0,5});
    trunk.setScale({0.4f, 0.4f, 5});
    leaves.setPosition({0, 0, 10});
    leaves.setScale({3, 3, 2});
}
