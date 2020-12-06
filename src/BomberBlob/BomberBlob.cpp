#include <cmath>

#include <BomberBlob/BomberBlob.hpp>
#include <BomberBlob/Textures.hpp>

#include <Blob/Core/Controls.hpp>
#include <imgui.h>

using namespace Blob;
using namespace Blob::Core;
using namespace Blob::Maths;
using namespace std;

BomberBlob::BomberBlob(Window &window, map<int, Player> &players)
    : Game(window, players, {{0, -20, 40}, {0, 0, 0}, {0, 0, 1}}), world(b2Vec2(0.0f, 0.0f)) {}

void BomberBlob::run() {
    std::cout << "bombers init" << std::endl;

    static const Vec2<float> pos[4] = {{width - 3, height - 3}, {-width + 3, -height + 3}, {width - 3, -height + 3}, {-width + 3, height - 3}};

    for (auto &p : players)
        bombers.emplace_back(world, pos[p.first], bombs, p.second, textures);

    // map init
    ground.primitive0.setMaterial(&textures.groundMat);
    // ground.setPosition({width / 2.f, height / 2.f, -1.f});
    ground.setPosition({0, 0, 0});
    ground.setScale({width, height, 1});
    textures.groundMat.texScale = {width, height};

    for (float i = -width + 5; i <= width - 4.5; i += 4.f) {
        for (float j = -height + 7; j <= height - 6.5; j += 4.f) {
            boxes.emplace_back(Vec2{i, j}, textures.boxMat, world);
        }
    }
    for (float i = -width + 7; i <= width - 6.5; i += 4.f) {
        for (float j = -height + 5; j <= height - 4.5; j += 4.f) {
            boxes.emplace_back(Vec2{i, j}, textures.boxMat, world);
        }
    }
    for (float i = -width + 9; i <= width - 8.5; i += 4.f) {
        boxes.emplace_back(Vec2{i, height - 3}, textures.boxMat, world);
        boxes.emplace_back(Vec2{i, -height + 3}, textures.boxMat, world);
    }
    for (float i = -height + 9; i <= height - 8.5; i += 4.f) {
        boxes.emplace_back(Vec2{width - 3, i}, textures.boxMat, world);
        boxes.emplace_back(Vec2{-width + 3, i}, textures.boxMat, world);
    }

    for (float i = -width + 1; i <= width - 0.5; i += 2.f) {
        indestructibleBoxes.emplace_back(Vec2{i, height - 1}, textures.indestructibleBoxMat, world);
        indestructibleBoxes.emplace_back(Vec2{i, -height + 1}, textures.indestructibleBoxMat, world);
    }

    for (float i = -height + 3; i <= height - 2.5; i += 2.f) {
        indestructibleBoxes.emplace_back(Vec2{width - 1, i}, textures.indestructibleBoxMat, world);
        indestructibleBoxes.emplace_back(Vec2{-width + 1, i}, textures.indestructibleBoxMat, world);
    }

    for (float i = -width + 5; i < width - 4.5f; i += 4) {
        for (float j = -height + 5; j < height - 4.5f; j += 4) {
            indestructibleBoxes.emplace_back(Vec2{i, j}, textures.indestructibleBoxMat, world);
        }
    }

    // Camera init
    float cameraAngle = PI / 4;
    window.setAngle(cameraAngle);
    // window.setOrthoProjection(std::sqrt(height*height + width*width));
    window.setRange(10, 100);

    // camera.setPosition({width / 2.f, 0, 1 + height / std::tan(cameraAngle)});
    // camera.setLookAt({width / 2.f, height * std::tan(cameraAngle / 2.f), 1});

    window.setCamera(camera);

    /*    Materials::PBR::light.position.x = 50;
        Materials::PBR::light.position.y = 50;
        Materials::PBR::light.position.z = 50.f;
        Materials::PBR::light.power = 500000.f;*/

    world.SetContactListener(&contactListener);
    // Start !!!
    gameLoop();
}

void BomberBlob::gameLoop() {

    bool endGame = false, escape = false, pauseMenu = false;
    float timeStep = 0;
    int32 velocityIterations = 6;
    int32 positionIterations = 2;

    // mainLoop
    while (window.isOpen() && !endGame) {
        window.draw(ground);

        // static objects
        for (auto &ib : indestructibleBoxes)
            window.draw(ib);

        for (auto i = bombs.begin(); i != bombs.end();) {
            if (i->update()) {
                i = bombs.erase(i);
            } else {
                window.draw(*i);
                i++;
            }
        }

        for (auto &b : bombers)
            b.updateInputs();

        DynamicCube::updateAll();

        world.Step(timeStep, velocityIterations, positionIterations);

        // evolutive objects :
        for (auto i = boxes.begin(); i != boxes.end();) {
            if (i->isDestroy()) {
                b2Vec2 position = i->body->GetPosition();
                bonus.emplace_back(Vec2{position.x, position.y}, textures, world);

                i = boxes.erase(i);
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

        // Info bar
        ImGui::SetNextWindowPos({0, 0});
        ImGui::Begin("InfoP1", nullptr,
                     ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                         ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBringToFrontOnFocus);

        int playerAlive = 0;
        for (const auto &b : bombers) {
            if (b.isAlive()) {
                window.draw(b);
                playerAlive++;
            }
            b.drawInfo();
        }

        ImGui::End();

        // pause menu
        if (pauseMenu) {
            ImGui::SetNextWindowPos(window.windowSize.cast<float>() / 2.f, 0, {0.5, 0.5});

            ImGui::Begin("Pause", nullptr,
                         ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                             ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBringToFrontOnFocus);

            ImGui::Text("   Pause   ");

            if (ImGui::Button("Resume", {ImGui::GetWindowContentRegionWidth(), 0})) {
                // collisionDetector.unpause();
                pauseMenu = false;
            }

            if (ImGui::Button("Main menu", {ImGui::GetWindowContentRegionWidth(), 0}))
                endGame = true;
            if (ImGui::Button("Quit", {ImGui::GetWindowContentRegionWidth(), 0})) {
                endGame = true;
                window.close();
            }

            ImGui::End();
        }

        // Score
        if (playerAlive <= 1) {
            // collisionDetector.pause();
            ImGui::SetNextWindowPos(window.windowSize.cast<float>() / 2, 0, {0.5, 0.5});

            ImGui::Begin("Score", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);

            for (const auto &b : bombers)
                if (b.isAlive()) {
                    b.getPlayer().score++;
                    ImGui::Text("Winner : %s", b.getPlayer().getName().c_str());
                }

            if (ImGui::Button("Main menu", {ImGui::GetWindowContentRegionWidth(), 0})) {
                endGame = true;
                // collisionDetector.unpause();
            }

            if (ImGui::Button("Quit", {ImGui::GetWindowContentRegionWidth(), 0})) {
                endGame = true;
                window.close();
            }

            ImGui::End();
        }

        // draw
        timeStep = window.display();

        // check imput
        if (window.keyboard.ESCAPE && !escape)
            escape = true;
        else if (!window.keyboard.ESCAPE && escape) {
            escape = false;
            if (pauseMenu) {
                pauseMenu = false;
                // collisionDetector.unpause();
            } else {
                pauseMenu = true;
                // collisionDetector.pause();
            }
        }
    }
}

void BomberBlob::settings() {
    //ImGui::Button("No");
}
