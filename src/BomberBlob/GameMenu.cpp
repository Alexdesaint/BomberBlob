#include <BomberBlob/GameMenu.hpp>

#include <Blob/Time.hpp>
#include <BomberBlob/BomberBlob.hpp>
#include <imgui.h>


using namespace Blob::Time;
using namespace Blob::GL;

GameMenu::GameMenu(Graphic &window) : window(window) {

	ImGuiIO &io = ImGui::GetIO();
	ImFont *font1 = io.Fonts->AddFontFromFileTTF("Data/fonts/PetMe.ttf", 16.f);
	ImFont *font2 = io.Fonts->AddFontFromFileTTF("Data/fonts/PetMe128.ttf", 48.f);

	ImGui::GetIO().FontDefault = font1;
	window.rebuildFontImGUI();

	while (window.isOpen()) {
		window.clear();

		ImGui::NewFrame();

		//ImGui::ShowDemoWindow();

		ImGui::PushFont(font2);

		ImGui::SetNextWindowPos(window.getSize()/2, 0, {0.5, 0.5});

		ImGui::Begin("main", nullptr,
					 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);

		ImGui::Text("BOMBERBLOB");

		ImGui::PopFont();
		ImGui::PushFont(font1);

		static std::vector<const char *> commands = {"None", "Keyboard", "Controller 1", "Controller 2"};
		static int selected1 = 1;
		ImGui::Combo("Player 1", &selected1, commands.data(), commands.size());

		//ImGui::SameLine();
		static int selected2 = 0;
		ImGui::Combo("Player 2", &selected2, commands.data(), commands.size());

		static int selected3 = 0;
		ImGui::Combo("Player 3", &selected3, commands.data(), commands.size());

		static int selected4 = 0;
		ImGui::Combo("Player 4", &selected4, commands.data(), commands.size());


		bool start = ImGui::Button("Start", {ImGui::GetContentRegionAvailWidth(), 0});
		//ImGui::PushItemWidth(ImGui::GetWindowWidth());
		if (ImGui::Button("Quit", {ImGui::GetContentRegionAvailWidth(), 0}))
			window.close();
		//ImGui::PopItemWidth();
		ImGui::End();


		ImGui::PopFont();
		window.drawImGUI();
		window.display();

		if (start)
			(BomberBlob(window));
	}
}
