#include <cmath>
#include <list>
#include <iostream>

#include <BomberBlob/BomberBlob.hpp>
#include <BomberBlob/Textures.hpp>

#include <imgui.h>

using namespace Blob;
using namespace std;

BomberBlob::BomberBlob(Blob::GL::Graphic &window) :
        window(window),
        width(21),
        height(15) {
    ImGuiIO &io = ImGui::GetIO();
    ImFont *font1 = io.Fonts->AddFontFromFileTTF("data/fonts/PetMe.ttf", 16.f);
    ImFont *font2 = io.Fonts->AddFontFromFileTTF("data/fonts/PetMe128.ttf", 48.f);

    ImGui::GetIO().FontDefault = font1;
    window.rebuildFontImGUI();


    while (window.isOpen()) {
        window.clear();

        ImGui::NewFrame();

        ImGui::PushFont(font2);

        ImGui::SetNextWindowPos(window.getSize() / 2, 0, {0.5, 0.5});

        ImGui::Begin("main", nullptr,
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);

        ImGui::Text("BOMBERBLOB");

        ImGui::PopFont();
        ImGui::PushFont(font1);

        ImGui::Text("Controllers connected : %zu", Blob::Controls::controllers.size());

        std::vector<const char *> commands = {"None", "Keyboard"};

        for (auto c : Blob::Controls::controllers) {
            commands.push_back(c->name.c_str());
        }

        std::string names[4] = {"Player 1", "Player 2", "Player 3", "Player 4"};
        float xpos[4] = {1.5f, width - 1.5f, width - 1.5f, 1.5f};
        float ypos[4] = {1.5f, height - 1.5f, 1.5f, height - 1.5f};
        static int selected[4] = {0, 0, 0, 0};

        for (int i = 0; i < 4; i++) {
            if (ImGui::Combo(names[i].c_str(), &selected[i], commands.data(), commands.size())) {
                auto it = players.find(i);

                switch (selected[i]) {
                    case 0:
                        if (it != players.end())
                            players.erase(it);
                        break;
                    case 1:
                        if (it == players.end())
                            it = players.emplace(std::piecewise_construct,
                                                 std::forward_as_tuple(i),
                                                 std::forward_as_tuple(xpos[i], ypos[i], bombs, names[i],
                                                                       textures)).first;

                        it->second.controller(false);
                        it->second.setKey(Player::right, &Controls::Keys::RIGHT);
                        it->second.setKey(Player::left, &Controls::Keys::LEFT);
                        it->second.setKey(Player::up, &Controls::Keys::UP);
                        it->second.setKey(Player::down, &Controls::Keys::DOWN);
                        it->second.setKey(Player::putBomb, &Controls::Keys::SPACE);
                        break;
                    default:
                        if (it == players.end())
                            it = players.emplace(std::piecewise_construct,
                                                 std::forward_as_tuple(i),
                                                 std::forward_as_tuple(xpos[i], ypos[i], bombs, names[i],
                                                                       textures)).first;

                        int contrl = selected[i] - 2;

                        if (contrl >= Blob::Controls::controllers.size()) {
                            players.erase(it);
                            throw Blob::Exception("Selected controller don't exist");
                        } else {
                            it->second.controller(true);
                            const Controls::Controller *c = *std::next(Controls::controllers.begin(), contrl);

                            if (c->joystickAxesCount < 2)
                                throw Blob::Exception("Controller don't have any joystick");

                            it->second.setJoystickAxe(Player::x, &c->joystickAxes[0]);
                            it->second.setJoystickAxe(Player::y, &c->joystickAxes[1]);
                            it->second.setButton(Player::putBomb, &c->buttons[0]);
                            if(c->buttonsCount >= 14) {
                                it->second.setButton(Player::up, &c->buttons[11]);
                                it->second.setButton(Player::right, &c->buttons[12]);
                                it->second.setButton(Player::down, &c->buttons[13]);
                                it->second.setButton(Player::left, &c->buttons[14]);
                            }
                        }
                        break;
                }
            }
        }

        bool start = ImGui::Button("Start", {ImGui::GetContentRegionAvailWidth(), 0});
        //ImGui::PushItemWidth(ImGui::GetWindowWidth());
        if (ImGui::Button("Quit", {ImGui::GetContentRegionAvailWidth(), 0}))
            window.close();
        //ImGui::PopItemWidth();
        ImGui::End();

        for (const auto &c : Blob::Controls::controllers)
            c->controllerOut();

        ImGui::PopFont();
        window.drawImGUI();
        window.display();

        if (start && players.size() > 1) {
            gameLoop();

            for(auto &p : players) {
                p.second.reset(xpos[p.first], ypos[p.first]);
            }

            bombs.clear();
        }
    }
}

void BomberBlob::gameLoop() {

    //map init
    ground.setTexture(textures.ground);
    ground.setPosition(width / 2.f, height / 2.f, 0);
    ground.setScale(width - 2, height - 2, 1);
    ground.setTextureScale({height - 2.f, width - 2.f});

    for (int i = 4; i < width - 4; i += 2) {
        boxs.emplace_back(0.5f + i, 0.5f + 1, textures.box);
        boxs.emplace_back(0.5f + i, height - 0.5f - 1, textures.box);
    }

    for (int i = 4; i < height - 4; i += 2) {
        boxs.emplace_back(0.5f + 1, 0.5f + i, textures.box);
        boxs.emplace_back(width - 0.5f - 1, 0.5f + i, textures.box);
    }

    for (int i = 3; i < width - 3; i += 2) {
        for (int j = 2; j < height - 2; j += 2) {
            boxs.emplace_back(0.5 + i, 0.5 + j, textures.box);
        }
    }

    for (int i = 2; i < width - 2; i += 2) {
        for (int j = 3; j < height - 3; j += 2) {
            boxs.emplace_back(0.5f + i, 0.5 + j, textures.box);
        }
    }

    for (int i = 0; i < width; i++) {
        indestructibleBoxs.emplace_back(0.5f + i, 0.5f, textures.indestructibleBox);
        indestructibleBoxs.emplace_back(0.5f + i, height - 0.5f, textures.indestructibleBox);
    }

    for (int i = 1; i < height - 1; i++) {
        indestructibleBoxs.emplace_back(0.5f, 0.5f + i, textures.indestructibleBox);
        indestructibleBoxs.emplace_back(width - 0.5f, 0.5f + i, textures.indestructibleBox);
    }

    for (int i = 2; i < width - 2; i += 2) {
        for (int j = 2; j < height - 2; j += 2) {
            indestructibleBoxs.emplace_back(0.5f + i, 0.5f + j, textures.indestructibleBox, 0.8);
        }
    }

    //Camera position
    float cameraAngle = PI / 4;

    window.setCameraAngle(cameraAngle);

    window.setCameraPosition(width / 2.f, 0, 1 + height / std::tan(cameraAngle));

    window.setCameraLookAt(width / 2.f, height * std::tan(cameraAngle / 2.f), 1);

    bool endGmae = false, escape = false, pauseMenu = false;

    //mainLoop
    while (window.isOpen() && !endGmae) {

        window.clear();

        window.draw(ground);

        //static objects
        for (auto &ib : indestructibleBoxs)
            window.draw(ib);

        for (auto i = bombs.begin(); i != bombs.end();) {
            if (i->update()) {
                i = bombs.erase(i);
            } else {
                window.draw(*i);
                i++;
            }
        }

        collisionDetector.update();

        //evolutive objects :
        for (auto i = boxs.begin(); i != boxs.end();) {
            if (i->isDestroy()) {
                bonus.emplace_back(i->position, textures);

                i = boxs.erase(i);
            } else {
                window.draw(*i);
                i++;
            }
        }

        for (auto i = bonus.begin(); i != bonus.end();) {
            if (i->update()) {
                i = bonus.erase(i);
            } else {
                window.draw(*i);
                i++;
            }
        }

        ImGui::NewFrame();
        ImGui::SetNextWindowPos({0, 0});
        ImGui::Begin("InfoP1", nullptr,
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove |
                     ImGuiWindowFlags_NoBringToFrontOnFocus);

        int playerAlive = 0;
        for (const auto &p : players) {
            if (p.second.isAlive()) {
                window.draw(p.second);
                playerAlive++;
            }
            p.second.drawInfo();
        }

        ImGui::End();

        //pause menu
        if (pauseMenu) {
            ImGui::SetNextWindowPos(window.getSize() / 2, 0, {0.5, 0.5});

            ImGui::Begin("Pause", nullptr,
                         ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);

            ImGui::Text("   Pause   ");

            if (ImGui::Button("Resume", {ImGui::GetContentRegionAvailWidth(), 0})) {
                collisionDetector.unpause();
                pauseMenu = false;
            }

            if (ImGui::Button("Main menu", {ImGui::GetContentRegionAvailWidth(), 0}))
                endGmae = true;
            if (ImGui::Button("Quit", {ImGui::GetContentRegionAvailWidth(), 0})) {
                endGmae = true;
                window.close();
            }

            ImGui::End();
        }


        //Score
        if (playerAlive <= 1) {
            collisionDetector.pause();
            ImGui::SetNextWindowPos(window.getSize() / 2, 0, {0.5, 0.5});

            ImGui::Begin("Score", nullptr,
                         ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);

            for (const auto &p : players)
                if (p.second.isAlive())
                    ImGui::Text("Winner : %s", p.second.getName().c_str());

            if (ImGui::Button("Main menu", {ImGui::GetContentRegionAvailWidth(), 0})) {
                endGmae = true;
                collisionDetector.unpause();
            }

            if (ImGui::Button("Quit", {ImGui::GetContentRegionAvailWidth(), 0})) {
                collisionDetector.unpause();
                endGmae = true;
                window.close();
            }

            ImGui::End();
        }

        //draw
        window.drawImGUI();
        window.display();

        //check imput
        if (Controls::Keys::ESCAPE && !escape) {
            escape = true;
        } else if (!Controls::Keys::ESCAPE && escape) {
            //endGmae = true;
            escape = false;
            if (pauseMenu) {
                pauseMenu = false;
                collisionDetector.unpause();
            } else {
                pauseMenu = true;
                collisionDetector.pause();
            }
        }
    }
}
