#include <utility>

#include <BomberBlob/Player.hpp>
#include <imgui.h>

using namespace Blob;

Player::Player(float x, float y, std::list<BombManager> &bombs, std::string name, Textures &textures) :
        textures(textures),
        RectDynamic({x, y}, {0.8f, 0.8f}, PLAYER),
        bombs(bombs),
        name(std::move(name)) {
    Cube::setPosition(x, y, 0.4f);
    setScale(0.8f, 0.8f, 0.8f);

    setTexture(textures.player);

    //init imput
    for(int i = 0; i < numOfActions; i++) {
        keys[i] = &defbool;
        buttons[i] = &defchar;
    }

    for(int i = 0; i < numOfAxes; i++)
        joystickAxes[i] = &deffloat;
}

void Player::preCollisionUpdate() {
    Vec2f Acceleration;
    bool pauseBomb;

    if (control) {
        if (*joystickAxes[x] * *joystickAxes[x] > 0.1)
            Acceleration.x = *joystickAxes[x];
        if (*joystickAxes[y] * *joystickAxes[y] > 0.1)
            Acceleration.y = -*joystickAxes[y];

        pauseBomb = *buttons[Actions::putBomb];

        if (*buttons[Actions::left])
            Acceleration.x -= 1;
        if (*buttons[Actions::right])
            Acceleration.x += 1;
        if (*buttons[Actions::down])
            Acceleration.y -= 1;
        if (*buttons[Actions::up])
            Acceleration.y += 1;
    } else {
        if (*keys[Actions::left])
            Acceleration.x -= 1;
        if (*keys[Actions::right])
            Acceleration.x += 1;
        if (*keys[Actions::down])
            Acceleration.y -= 1;
        if (*keys[Actions::up])
            Acceleration.y += 1;

        pauseBomb = *keys[Actions::putBomb];
    }

    if (speed.length2() < deceleration * deceleration)
        speed.reset();
    else
        speed.setLength(speed.length() - deceleration);

    if (!Acceleration.isNull()) {
        speed += Acceleration.setLength(acceleration);
        if (speed.length2() > maxSpeed * maxSpeed)
            speed.setLength(maxSpeed);
    }

    if (pauseBomb && !onBomb && bombPosed < maxBomb) {
        auto here = getImtemsHere(position);

        bool noBomb = true;
        for (const auto &i : here) {
            if (i->getObjectType() == BOMB) {
                noBomb = false;
                break;
            }
        }

        if (noBomb) {
            bombs.emplace_front(position.cast<int>().cast<float>() + 0.5f, *this, textures);
            lastBomb = &bombs.front();
            onBomb = true;
            bombPosed++;
        }
    }
}

void Player::postCollisionUpdate() {
    Cube::setPosition(position, 0.4f);

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
        disableCollision();
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

const std::string &Player::getName() const {
    return name;
}

void Player::setName(const std::string &n) {
    Player::name = n;
}

void Player::drawInfo() const {
    ImGui::Text("%s", name.c_str());
    ImGui::Image(&textures.extraBomb, textures.extraBomb.getTextureSize());
    ImGui::SameLine();
    ImGui::Text("%u", maxBomb);
    ImGui::Image(&textures.extraPower, textures.extraPower.getTextureSize());
    ImGui::SameLine();
    ImGui::Text("%.1f", bombPower);
    ImGui::Image(&textures.extraSpeed, textures.extraSpeed.getTextureSize());
    ImGui::SameLine();
    ImGui::Text("%.1f", maxSpeed);
}

void Player::reset(float x, float y) {
    disableCollision();
    position = {x, y};
    alive = false;
    acceleration = 2.5f;
    deceleration = 1.5f;
    maxSpeed = 2.5f;
    bombPower = 2.f;
    alive = true;
    onBomb = false;
    enableCollision();
}
