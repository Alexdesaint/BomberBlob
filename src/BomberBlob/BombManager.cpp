#include <BomberBlob/BombManager.hpp>

#include <BomberBlob/Bomber.hpp>

using namespace Blob;
using namespace Blob::Time;

BombManager::BombManager(b2World &world,
                         const Vec2<float> &pos,
                         Bomber &bomber) :
    bomber(bomber), world(world) {
    bomb = new Bomb(pos, world);

    addChild(bomb);

    start = now();
}

bool BombManager::update() {
    Duration flow = now() - start;

    if (bomb == nullptr) {
        if (exUP != nullptr && !exUP->isActive()) {
            removeChild(exUP);
            delete exUP;
            exUP = nullptr;
        }

        if (exDO != nullptr && !exDO->isActive()) {
            removeChild(exDO);
            delete exDO;
            exDO = nullptr;
        }

        if (exLE != nullptr && !exLE->isActive()) {
            removeChild(exLE);
            delete exLE;
            exLE = nullptr;
        }

        if (exRI != nullptr && !exRI->isActive()) {
            removeChild(exRI);
            delete exRI;
            exRI = nullptr;
        }

        return exUP == nullptr && exDO == nullptr && exRI == nullptr &&
               exLE == nullptr;
    } else if (flow.count() > bombDelay || bomb->isDestroyed()) {
        bomber.bombPosed--;

        b2Vec2 p = bomb->body->GetPosition();
        Blob::Vec2<float> position = {p.x, p.y};
        exRI =
            new Explosion(world, position, Vec2<float>(0, 1), bomber.bombPower);
        exLE = new Explosion(world,
                             position,
                             Vec2<float>(0, -1),
                             bomber.bombPower);
        exDO =
            new Explosion(world, position, Vec2<float>(1, 0), bomber.bombPower);
        exUP = new Explosion(world,
                             position,
                             Vec2<float>(-1, 0),
                             bomber.bombPower);

        addChild(exRI);
        addChild(exLE);
        addChild(exDO);
        addChild(exUP);

        removeChild(bomb);
        delete bomb;
        bomb = nullptr;
    }

    return false;
}

Bomb *BombManager::getBomb() const {
    return bomb;
}

BombManager::~BombManager() {
    if (exUP != nullptr) {
        removeChild(exUP);
        delete exUP;
    }

    if (exDO != nullptr) {
        removeChild(exDO);
        delete exDO;
    }

    if (exLE != nullptr) {
        removeChild(exLE);
        delete exLE;
    }

    if (exRI != nullptr) {
        removeChild(exRI);
        delete exRI;
    }

    if (bomb != nullptr) {
        removeChild(bomb);
        delete bomb;
    }
}
