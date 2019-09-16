#include <Menu.hpp>

#include <imgui.h>
#include <Blob/Exception.hpp>
#include <BomberBlob/BomberBlob.hpp>

using namespace Blob;
using namespace std;

Menu::Menu(Blob::GL::Graphic &window) : window(window) {
	ImGuiIO &io = ImGui::GetIO();
	ImFont *font1 = io.Fonts->AddFontFromFileTTF("data/fonts/PetMe.ttf", 16.f);
	ImFont *font2 = io.Fonts->AddFontFromFileTTF("data/fonts/PetMe128.ttf", 48.f);

	ImGui::GetIO().FontDefault = font1;
	window.rebuildFontImGUI();

	while (window.isOpen()) {
		window.clear();

		ImGui::NewFrame();


		ImGui::ShowDemoWindow();

		ImGui::SetNextWindowPos(window.getSize() / 2, 0, {0.5, 0.5});

		ImGui::PushFont(font2);

		ImGui::Begin("main", nullptr,
					 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
					 ImGuiWindowFlags_AlwaysAutoResize);

		ImGui::Text("BOMBERBLOB");

		ImGui::PopFont();
		ImGui::PushFont(font1);

		if (ImGui::BeginTabBar("bar", ImGuiTabBarFlags_None)) {
			if (ImGui::BeginTabItem("Players")) {
				playerSelection();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Game settings")) {
				ImGui::Text("Game settings");
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}

		ImGui::Separator();

		bool start = ImGui::Button("Start", {ImGui::GetContentRegionAvailWidth(), 0});
		if (ImGui::Button("Quit", {ImGui::GetWindowContentRegionWidth(), 0}))
			window.close();
		ImGui::End();

		for (const auto &c : Blob::Controls::controllers)
			c->controllerOut();

		ImGui::PopFont();
		window.drawImGUI();
		window.display();

		if (start && players.size() > 1)
			(BomberBlob(window, players, textures));
	}
}

void Menu::playerSelection() {
	ImGui::Text("Controllers connected : %zu", Blob::Controls::controllers.size());

	std::vector<const char *> commands = {"None", "Keyboard"};

	for (auto c : Blob::Controls::controllers)
		commands.push_back(c->name.c_str());

	static const std::string names[4] = {"Player 1", "Player 2", "Player 3", "Player 4"};
	static int selected[4] = {0, 0, 0, 0};

	for (int i = 0; i < 4; i++) {
		ImGui::SetNextItemWidth(300);
		if (ImGui::BeginCombo(names[i].c_str(), commands[selected[i]])) {
			int oldSelected = selected[i];
			for (size_t j = 0; j < commands.size(); j++) {
				if (ImGui::Selectable(commands[j], oldSelected == j))
					selected[i] = j;
				if (oldSelected == j)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();

			if (oldSelected != selected[i]) {
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
												 std::forward_as_tuple(names[i], i)).first;

						it->second.controller(false);
						it->second.setKey(Player::right, &Controls::Keys::RIGHT);
						it->second.setKey(Player::left, &Controls::Keys::LEFT);
						it->second.setKey(Player::up, &Controls::Keys::UP);
						it->second.setKey(Player::down, &Controls::Keys::DOWN);
						it->second.setKey(Player::action, &Controls::Keys::SPACE);
						break;
					default:
						if (it == players.end())
							it = players.emplace(std::piecewise_construct,
												 std::forward_as_tuple(i),
												 std::forward_as_tuple(names[i], i)).first;

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
							it->second.setButton(Player::action, &c->buttons[0]);
							if (c->buttonsCount >= 14) {
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
	}
}
