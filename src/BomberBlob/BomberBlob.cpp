#include <cmath>
#include <list>
#include <iostream>

#include <BomberBlob/BomberBlob.hpp>

#include <BomberBlob/Explosion.hpp>
#include <BomberBlob/Player.hpp>
#include <BomberBlob/IndestructibleBox.hpp>
#include <BomberBlob/Box.hpp>
#include <BomberBlob/InfoBar.hpp>
#include <BomberBlob/BombManager.hpp>
#include <BomberBlob/Bonus.hpp>

#include <imgui.h>

using namespace Blob;

BomberBlob::BomberBlob(GL::Graphic &window) {

	//player init
	Player player(1.5f, 1.5f, bombs);
	player.setAction(Player::right, &keys[GL::Key::RIGHT]);
	player.setAction(Player::left, &keys[GL::Key::LEFT]);
	player.setAction(Player::up, &keys[GL::Key::UP]);
	player.setAction(Player::down, &keys[GL::Key::DOWN]);
	player.setAction(Player::putBomb, &keys[GL::Key::SPACE]);

	//map init
	ground.loadBMP("data/Grass.bmp");
	ground.setPosition(width / 2.f, height / 2.f, 0);
	ground.setScale(width - 2, height - 2, 1);
	ground.setTextureScale({height - 2.f, width - 2.f});

	for (int i = 4; i < width - 4; i += 2) {
		boxs.emplace_back(0.5f + i, 0.5f + 1);
		boxs.emplace_back(0.5f + i, height - 0.5f - 1);
	}

	for (int i = 4; i < height - 4; i += 2) {
		boxs.emplace_back(0.5f + 1, 0.5f + i);
		boxs.emplace_back(width - 0.5f - 1, 0.5f + i);
	}

	for (int i = 3; i < width - 3; i += 2) {
		for (int j = 2; j < height - 2; j += 2) {
			boxs.emplace_back(0.5 + i, 0.5 + j);
		}
	}

	for (int i = 2; i < width - 2; i += 2) {
		for (int j = 3; j < height - 3; j += 2) {
			boxs.emplace_back(0.5f + i, 0.5 + j);
		}
	}

	for (int i = 0; i < width; i += 1) {
		indestructibleBoxs.emplace_back(0.5f + i, 0.5f);
	}

	for (int i = 0; i < width; i += 1) {
		indestructibleBoxs.emplace_back(0.5f + i, height - 0.5f);
	}

	for (int i = 1; i < height - 1; i += 1) {
		indestructibleBoxs.emplace_back(0.5f, 0.5f + i);
	}

	for (int i = 1; i < height - 1; i += 1) {
		indestructibleBoxs.emplace_back(width - 0.5f, 0.5f + i);
	}

	for (int i = 2; i < width - 2; i += 2) {
		for (int j = 2; j < height - 2; j += 2) {
			indestructibleBoxs.emplace_back(0.5f + i, 0.5f + j, 0.8);
		}
	}

	//pannel init
	speedTex.loadBMP("data/ExtraSpeed.bmp", true);
	explosionRangeTex.loadBMP("data/ExtraPower.bmp", true);
	maxBombTex.loadBMP("data/ExtraBomb.bmp", true);

	//Camera position
	float cameraAngle = PI / 4;

	window.setCameraAngle(cameraAngle);

	window.setCameraPosition(width / 2.f, 0, 1 + height / std::tan(cameraAngle));

	window.setCameraLookAt(width / 2.f, height * std::tan(cameraAngle / 2.f), 1);

	bool endGmae = false, escape = false, pauseMenu = false;

	//mainLoop
	while(window.isOpen() && !endGmae) {

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
                bonus.emplace_back(i->getPosition());

				i = boxs.erase(i);
			}
			else {
				window.draw(*i);
				i++;
			}
		}

		for (auto i = bonus.begin(); i != bonus.end();) {
			if (i->update()) {
				i = bonus.erase(i);
			}
			else {
				window.draw(*i);
				i++;
			}
		}

		window.draw(player);

		//side panel
		ImGui::NewFrame();

		ImGui::SetNextWindowPos({0, 0});
		ImGui::Begin("InfoP1", nullptr,
					 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove |
					 ImGuiWindowFlags_NoBringToFrontOnFocus);

		ImGui::Text("Player 1");
		ImGui::Image(&maxBombTex, maxBombTex.getTextureSize()); ImGui::SameLine();
		ImGui::Text("%u", player.getMaxBomb());
		ImGui::Image(&explosionRangeTex, explosionRangeTex.getTextureSize()); ImGui::SameLine();
		ImGui::Text("%.1f", player.getBombPower());
		ImGui::Image(&speedTex, speedTex.getTextureSize()); ImGui::SameLine();
		ImGui::Text("%.1f", player.getMaxSpeed());

		ImGui::End();

		//pause menu
		if (pauseMenu) {
			ImGui::SetNextWindowPos(window.getSize()/2, 0, {0.5, 0.5});

			ImGui::Begin("Pause", nullptr,
						 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);

			ImGui::Text("   Pause   ");

			if (ImGui::Button("Resume", {ImGui::GetContentRegionAvailWidth(), 0})) {
				collisionDetector.unpause();
				pauseMenu = false;
			}

			if (ImGui::Button("Main menu", {ImGui::GetContentRegionAvailWidth(), 0}))
				endGmae = true;

			ImGui::End();
		}

		//draw
		window.drawImGUI();
		window.display();

		//if(!player.isAlive())
		//	endGmae = true;

		//check imput
		if (keys[GL::ESCAPE] && !escape) {
			escape = true;
		} else if (!keys[GL::ESCAPE] && escape) {
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
