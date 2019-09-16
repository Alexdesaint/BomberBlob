#include <cmath>

#include <BomberBlob/BomberBlob.hpp>
#include <BomberBlob/Textures.hpp>

#include <imgui.h>

using namespace Blob;
using namespace std;

BomberBlob::BomberBlob(GL::Graphic &window, map<int, Player> &players, Textures &textures) :
		Game(window, players), textures(textures) {}

void BomberBlob::start() {
//bombers init
	static const Vec2f pos[4] = {
			{1.5f,         1.5f},
			{width - 1.5f, height - 1.5f},
			{width - 1.5f, 1.5f},
			{1.5f,         height - 1.5f}
	};

	for(auto &p : players)
		bombers.emplace_back(pos[p.first], bombs, p.second, textures);

	//map init
	ground.setTexture(textures.ground);
	ground.setPosition(width / 2.f, height / 2.f, 0);
	ground.setScale(width - 2, height - 2, 1);
	ground.setTextureScale({height - 2.f, width - 2.f});

	for (int i = 4; i < width - 4; i += 2) {
		boxes.emplace_back(0.5f + i, 0.5f + 1, textures.box);
		boxes.emplace_back(0.5f + i, height - 0.5f - 1, textures.box);
	}

	for (int i = 4; i < height - 4; i += 2) {
		boxes.emplace_back(0.5f + 1, 0.5f + i, textures.box);
		boxes.emplace_back(width - 0.5f - 1, 0.5f + i, textures.box);
	}

	for (int i = 3; i < width - 3; i += 2) {
		for (int j = 2; j < height - 2; j += 2) {
			boxes.emplace_back(0.5 + i, 0.5 + j, textures.box);
		}
	}

	for (int i = 2; i < width - 2; i += 2) {
		for (int j = 3; j < height - 3; j += 2) {
			boxes.emplace_back(0.5f + i, 0.5 + j, textures.box);
		}
	}

	for (int i = 0; i < width; i++) {
		indestructibleBoxes.emplace_back(0.5f + i, 0.5f, textures.indestructibleBox);
		indestructibleBoxes.emplace_back(0.5f + i, height - 0.5f, textures.indestructibleBox);
	}

	for (int i = 1; i < height - 1; i++) {
		indestructibleBoxes.emplace_back(0.5f, 0.5f + i, textures.indestructibleBox);
		indestructibleBoxes.emplace_back(width - 0.5f, 0.5f + i, textures.indestructibleBox);
	}

	for (int i = 2; i < width - 2; i += 2) {
		for (int j = 2; j < height - 2; j += 2) {
			indestructibleBoxes.emplace_back(0.5f + i, 0.5f + j, textures.indestructibleBox, 0.8);
		}
	}

	//Camera init
	float cameraAngle = PI / 4;

	window.setCameraAngle(cameraAngle);

	window.setCameraPosition(width / 2.f, 0, 1 + height / std::tan(cameraAngle));

	window.setCameraLookAt(width / 2.f, height * std::tan(cameraAngle / 2.f), 1);

	// Start !!!
	gameLoop();
}

void BomberBlob::gameLoop() {

	bool endGame = false, escape = false, pauseMenu = false;

	//mainLoop
	while (window.isOpen() && !endGame) {

		window.clear();

		window.draw(ground);

		//static objects
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

		collisionDetector.update();

		//evolutive objects :
		for (auto i = boxes.begin(); i != boxes.end();) {
			if (i->isDestroy()) {
				bonus.emplace_back(i->position, textures);

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
		ImGui::NewFrame();
		ImGui::SetNextWindowPos({0, 0});
		ImGui::Begin("InfoP1", nullptr,
					 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
					 ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize |
					 ImGuiWindowFlags_NoBringToFrontOnFocus);

		int playerAlive = 0;
		for (const auto &b : bombers) {
			if (b.isAlive()) {
				window.draw(b);
				playerAlive++;
			}
			b.drawInfo();
		}

		ImGui::End();

		//pause menu
		if (pauseMenu) {
			ImGui::SetNextWindowPos(window.getSize() / 2, 0, {0.5, 0.5});

			ImGui::Begin("Pause", nullptr,
						 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
						 ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize |
						 ImGuiWindowFlags_NoBringToFrontOnFocus);

			ImGui::Text("   Pause   ");

			if (ImGui::Button("Resume", {ImGui::GetContentRegionAvailWidth(), 0})) {
				collisionDetector.unpause();
				pauseMenu = false;
			}

			if (ImGui::Button("Main menu", {ImGui::GetContentRegionAvailWidth(), 0}))
				endGame = true;
			if (ImGui::Button("Quit", {ImGui::GetContentRegionAvailWidth(), 0})) {
				endGame = true;
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

			for (const auto &b : bombers)
				if (b.isAlive()) {
					b.getPlayer().score++;
					ImGui::Text("Winner : %s", b.getPlayer().getName().c_str());
				}

			if (ImGui::Button("Main menu", {ImGui::GetContentRegionAvailWidth(), 0})) {
				endGame = true;
				collisionDetector.unpause();
			}

			if (ImGui::Button("Quit", {ImGui::GetContentRegionAvailWidth(), 0})) {
				endGame = true;
				window.close();
			}

			ImGui::End();
		}

		//draw
		window.drawImGUI();
		window.display();

		//check imput
		if (Controls::Keys::ESCAPE && !escape)
			escape = true;
		else if (!Controls::Keys::ESCAPE && escape) {
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

void BomberBlob::settings() {
	ImGui::Button("No");
}
