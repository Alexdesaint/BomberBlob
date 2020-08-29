#include <BomberBlob/Bomber.hpp>

#include <imgui.h>

using namespace Blob;

Bomber::Bomber(Vec2f pos, std::list<BombManager> &bombs, Player &player, Textures &textures, float scale) :
        textures(textures),
        player(player),
        RectDynamic(pos, {scale, scale}, PLAYER),
        bombs(bombs),
        Blob::Shape(textures.player[player.number], pos.x, pos.y, scale/2.f, scale/2.f, scale/2.f, scale/2.f){
}

void Bomber::preCollisionUpdate() {
    Vec2f Acceleration;
    bool pauseBomb;

    if (player.control) {
        if (*player.joystickAxes[Player::x] * *player.joystickAxes[Player::x] > 0.1)
            Acceleration.x = *player.joystickAxes[Player::x];
        if (*player.joystickAxes[Player::y] * *player.joystickAxes[Player::y] > 0.1)
            Acceleration.y = -*player.joystickAxes[Player::y];

        pauseBomb = *player.buttons[Player::action];

        if (*player.buttons[Player::left])
            Acceleration.x -= 1;
        if (*player.buttons[Player::right])
            Acceleration.x += 1;
        if (*player.buttons[Player::down])
            Acceleration.y -= 1;
        if (*player.buttons[Player::up])
            Acceleration.y += 1;
    } else {
        if (*player.keys[Player::left])
            Acceleration.x -= 1;
        if (*player.keys[Player::right])
            Acceleration.x += 1;
        if (*player.keys[Player::down])
            Acceleration.y -= 1;
        if (*player.keys[Player::up])
            Acceleration.y += 1;

        pauseBomb = *player.keys[Player::action];
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

void Bomber::postCollisionUpdate() {
    setPosition(position, 0.4f);

    if (onBomb) {
        Bomb *bomb = lastBomb->getBomb();
        if (bomb == nullptr || !bomb->overlap(*this)) {
            onBomb = false;
        }
    }
}

void Bomber::hit(const int objectType, Object &object) {
    if (onBomb && objectType == BOMB && &object == lastBomb->getBomb()) {
        setReaction(IGNORE);
        return;
    }
    if (objectType == EXTRABOMB) {
        maxBomb++;
    } else if (objectType == EXTRASPEED) {
        maxSpeed += 0.25;
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

float Bomber::getMaxSpeed() const {
    return maxSpeed;
}

float Bomber::getBombPower() const {
    return bombPower;
}

unsigned int Bomber::getMaxBomb() const {
    return maxBomb;
}

void Bomber::drawInfo() const {
	//ImGui::Image(&textures.player[player.number], textures.player[player.number].getTextureSize());
	ImGui::SameLine();
    ImGui::Text("%s", player.name.c_str());
    //ImGui::Image(&textures.extraBomb, textures.extraBomb.getTextureSize());
    ImGui::SameLine();
    ImGui::Text("%u", maxBomb);
    //ImGui::Image(&textures.extraPower, textures.extraPower.getTextureSize());
    ImGui::SameLine();
    ImGui::Text("%.1f", bombPower);
    //ImGui::Image(&textures.extraSpeed, textures.extraSpeed.getTextureSize());
    ImGui::SameLine();
    ImGui::Text("%.2f", maxSpeed);
}

Player &Bomber::getPlayer() const {
	return player;
}
