#include <utility>

#include <BomberBlob/Player.hpp>
#include <imgui.h>

using namespace Blob;

Player::Player(float x, float y, std::list<BombManager> &bombs, std::string name) :
        RectDynamic({x, y}, {0.8f, 0.8f}, PLAYER), bombs(bombs),
        name(std::move(name)) {
    Cube::setPosition(x, y, 0.4f);
    setScale(0.8f, 0.8f, 0.8f);

    texture.setColor(255, 255, 255);
    setTexture(texture);
}

void Player::preCollisionUpdate() {
    Vec2f Acceleration;

    if (*keys[Actions::left]) {
        Acceleration.x -= 1;
    }
    if (*keys[Actions::right]) {
        Acceleration.x += 1;
    }
    if (*keys[Actions::down]) {
        Acceleration.y -= 1;
    }
    if (*keys[Actions::up]) {
        Acceleration.y += 1;
    }

    if (!Acceleration.isNull()) {
        speed = Acceleration.setLength(maxSpeed);
    } else
        speed.reset();

    if (*keys[Actions::putBomb] && !onBomb && bombPosed < maxBomb) {
        auto here = getImtemsHere(getPosition());

        bool noBomb = true;
        for (const auto &i : here) {
            if (i->getObjectType() == BOMB) {
                noBomb = false;
                break;
            }
        }

        if (noBomb) {
            bombs.emplace_front(getPosition().cast<int>().cast<float>() + 0.5f, *this);
            lastBomb = &bombs.front();
            onBomb = true;
            bombPosed++;
        }
    }
}

void Player::postCollisionUpdate() {
    Cube::setPosition(getPosition(), 0.4f);

    if (onBomb) {
        Bomb *bomb = lastBomb->getBomb();
        if (bomb == nullptr || !bomb->overlap(*this)) {
            onBomb = false;
        }
    }
}

void Player::hit(const int objectType, Object &object) {
    if (onBomb && objectType == BOMB && &object == lastBomb->getBomb()) {
        setReaction(IGNORE);
        return;
    }
    if (objectType == EXTRABOMB) {
        maxBomb++;
    } else if (objectType == EXTRASPEED) {
        maxSpeed += 1;
    } else if (objectType == EXTRAPOWER) {
        bombPower++;
    } else if (objectType == EXPLOSION) {
        alive = false;
        setReaction(IGNORE);
        return;
    }

    setReaction(STOP);
}

float Player::getMaxSpeed() const {
    return maxSpeed;
}

float Player::getBombPower() const {
    return bombPower;
}

unsigned int Player::getMaxBomb() const {
    return maxBomb;
}

bool Player::isConnected() const {
    return connected;
}

void Player::setConnected(bool c) {
    Player::connected = c;
}

const std::string &Player::getName() const {
    return name;
}

void Player::setName(const std::string &n) {
    Player::name = n;
}

void Player::drawInfo() const {
    static Blob::GL::Texture speedTex("data/ExtraSpeed.bmp", true),
            explosionRangeTex("data/ExtraPower.bmp", true),
            maxBombTex("data/ExtraBomb.bmp", true);

    ImGui::Text("%s", name.c_str());
    ImGui::Image(&maxBombTex, maxBombTex.getTextureSize());
    ImGui::SameLine();
    ImGui::Text("%u", maxBomb);
    ImGui::Image(&explosionRangeTex, explosionRangeTex.getTextureSize());
    ImGui::SameLine();
    ImGui::Text("%.1f", bombPower);
    ImGui::Image(&speedTex, speedTex.getTextureSize());
    ImGui::SameLine();
    ImGui::Text("%.1f", maxSpeed);
}
