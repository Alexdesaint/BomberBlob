#include <BomberBlob/Bomber.hpp>

#include <imgui.h>

using namespace Blob;

Bomber::Bomber(b2World &world, const Blob::Vec2<float> &pos, std::list<BombManager> &bombs, Player &player, Textures &textures, float scale)
    : textures(textures), player(player), bombs(bombs), world(world), DynamicCube(world, pos, scale, material, UserData::PLAYER),
      material(player.color) {}

void Bomber::updateInputs() {
    speed = 0;
    bool pauseBomb;

    if (player.control) {
        if (*player.joystickAxes[Player::x] * *player.joystickAxes[Player::x] > 0.1)
            speed.x = *player.joystickAxes[Player::x];
        if (*player.joystickAxes[Player::y] * *player.joystickAxes[Player::y] > 0.1)
            speed.y = -*player.joystickAxes[Player::y];

        pauseBomb = *player.buttons[Player::action];

        if (*player.buttons[Player::left])
            speed.x -= 1;
        if (*player.buttons[Player::right])
            speed.x += 1;
        if (*player.buttons[Player::down])
            speed.y -= 1;
        if (*player.buttons[Player::up])
            speed.y += 1;
    } else {
        if (*player.keys[Player::left])
            speed.x -= 1;
        if (*player.keys[Player::right])
            speed.x += 1;
        if (*player.keys[Player::down])
            speed.y -= 1;
        if (*player.keys[Player::up])
            speed.y += 1;

        pauseBomb = *player.keys[Player::action];
    }

    if(!speed.isNull())
        speed.setLength(maxSpeed);

    /*if (speed.length2() < deceleration * deceleration)
        speed.reset();
    else
        speed.setLength(speed.length() - deceleration);

    if (!Acceleration.isNull()) {
        speed += Acceleration.setLength(acceleration);
        if (speed.length2() > maxSpeed * maxSpeed)
            speed.setLength(maxSpeed);
    }*/

    if (pauseBomb && !onBomb && bombPosed < maxBomb) {
        /*        auto here = getImtemsHere(position);

                bool noBomb = true;
                for (const auto &i : here) {
                    if (i->getObjectType() == BOMB) {
                        noBomb = false;
                        break;
                    }
                }

                if (noBomb) {*/
        bombs.emplace_front(world, position.cast<int>().cast<float>() + 0.5f, *this, textures);
        lastBomb = &bombs.front();
        // onBomb = true;
        bombPosed++;
        //        }
    }

    setPosition({position, 0.4f});

    /*    if (onBomb) {
            Bomb *bomb = lastBomb->getBomb();
            if (bomb == nullptr || !bomb->overlap(*this)) {
                onBomb = false;
            }
        }*/
}

void Bomber::hit(Collider *c) {
    if (onBomb && c->id == BOMB && c == lastBomb->getBomb()) {
        //setReaction(IGNORE);
        return;
    }
    if (c->id == EXTRABOMB) {
        maxBomb++;
    } else if (c->id == EXTRASPEED) {
        maxSpeed += 0.25;
    } else if (c->id == EXTRAPOWER) {
        bombPower++;
    } else if (c->id == EXPLOSION) {
        alive = false;
        //setReaction(IGNORE);
        //disableCollision();
        return;
    }

    // setReaction(STOP);
}

void Bomber::drawInfo() const {
    // ImGui::Image(&textures.player[player.number], textures.player[player.number].getTextureSize());
    ImGui::SameLine();
    ImGui::Text("%s", player.name.c_str());
    // ImGui::Image(&textures.extraBomb, textures.extraBomb.getTextureSize());
    ImGui::SameLine();
    ImGui::Text("%u", maxBomb);
    // ImGui::Image(&textures.extraPower, textures.extraPower.getTextureSize());
    ImGui::SameLine();
    ImGui::Text("%.1f", bombPower);
    // ImGui::Image(&textures.extraSpeed, textures.extraSpeed.getTextureSize());
    ImGui::SameLine();
    ImGui::Text("%.2f", maxSpeed);
}

Player &Bomber::getPlayer() const {
    return player;
}
