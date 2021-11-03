#include <Menu.hpp>

#include <Blob/Core/Controls.hpp>
#include <Blob/Core/Exception.hpp>
#include <BlobJump/BlobJump.hpp>
#include <BlobSurvive/BlobSurvive.hpp>
#include <BomberBlob/BomberBlob.hpp>
#include <imgui.h>

using namespace Blob;
using namespace std;

struct ComboMenu {
    size_t selected = 0; /// item selected
    // std::map<size_t, std::string> &options;
    bool updated = false;
    std::string name;

    void show(std::vector<std::string> options) {
        if (options.empty()) {
            if (ImGui::BeginCombo(name.c_str(), "")) {
                // ImGui::Selectable("", false);
                ImGui::EndCombo();
            }

            return;
        }

        if (selected >= options.size())
            selected = options.size() - 1;

        updated = false;
        if (ImGui::BeginCombo(name.c_str(), options[selected].c_str())) {
            int oldSelected = selected;
            for (size_t j = 0; j < options.size(); j++) {
                if (ImGui::Selectable(options[j].c_str(), oldSelected == j)) {
                    if (oldSelected != j) {
                        selected = j;
                        updated = true;
                    }
                }
            }
            ImGui::EndCombo();
        }
    }
};

Menu::Menu(Window &window) : window(window) {
    ImGui::ShowDemoWindow();
    ImGuiIO &io = ImGui::GetIO();
    // ImFont *font1 =
    // io.Fonts->AddFontFromFileTTF("data/fonts/PetMe.ttf", 16.f); ImFont *font2
    // = io.Fonts->AddFontFromFileTTF("data/fonts/PetMe128.ttf", 48.f);

    // ImGui::GetIO().FontDefault = font1;
    // window.rebuildFontImGUI();
    ComboMenu comboGame{0, false, "Demo"};
    std::vector<std::string> games{"BlobJump", "BomberBlob", "BlobSurvive"};

    Game *game = new BlobJump(window, players);
    // window.setCamera(game->camera);

    while (window.isOpen()) {

        // ImGui::SetNextWindowPos((window.windowSize / 2).cast<float>(), 0,
        // {0.5, 0.5});

        // ImGui::PushFont(font2);

        ImGui::Begin("BlobEngine Demo",
                     nullptr,
                     ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                         ImGuiWindowFlags_AlwaysAutoResize);

        comboGame.show(games);
        if (comboGame.updated) {
            delete game;
            switch (comboGame.selected) {
            case 1:
                game = new BomberBlob(window, players);
                break;
            case 2:
                game = new BlobSurvive(window, players);
                break;
            default:
                game = new BlobJump(window, players);
                break;
            }
            // window.setCamera(game->camera);
        }

        // ImGui::PopFont();
        // ImGui::PushFont(font1);

        if (ImGui::CollapsingHeader("Players"))
            playerSelection();
        if (ImGui::CollapsingHeader("Settings"))
            game->settings();

        ImGui::Separator();

        if (game->loadReady())
            if (ImGui::Button("Load",
                              {ImGui::GetWindowContentRegionWidth(), 0}))
                game->load();
        bool start = false;
        if (game->gameReady())
            start = ImGui::Button("Start",
                                  {ImGui::GetWindowContentRegionWidth(), 0});

        /*        if (start && players.size() <= 1) {
                    std::cout << "not enough players" << std::endl;
                    start = false;
                }*/

        if (ImGui::Button("Quit", {ImGui::GetWindowContentRegionWidth(), 0}))
            window.close();
        ImGui::End();

        // ImGui::PopFont();
        window.display();

        if (start)
            game->run();
    }

    delete game;
}

void Menu::playerSelection() {
    static Color::RGB colors[]{
        Blob::Color::Yellow,
        Blob::Color::Brown,
        Blob::Color::Gray,
        Blob::Color::Red,
        Blob::Color::Orange,
        Blob::Color::Green,
        Blob::Color::Black,
    };

    static map<size_t, ComboMenu> comboMenus;
    std::vector<std::string> commands = {"None", "Keyboard"};
    if (comboMenus.find(0) == comboMenus.end())
        comboMenus.emplace(0, ComboMenu{1, true, "Player 0"});

    for (size_t i = 0; i < GLFW::Window::joystickCount; i++) {
        if (GLFW::Window::joystickConnected[i]) {
            if (comboMenus.find(i + 1) == comboMenus.end())
                comboMenus.emplace(
                    i + 1,
                    ComboMenu{i + 2, true, "Player " + std::to_string(i + 1)});
            commands.emplace_back(GLFW::Window::joystickName[i]);
        }
    }

    for (auto &[key, comboMenu] : comboMenus) {
        ImGui::SetNextItemWidth(300);
        comboMenu.show(commands);
    }

    for (auto &[key, comboMenu] : comboMenus) {
        if (comboMenu.updated) {
            comboMenu.updated = false;
            auto it = players.find(key);

            switch (comboMenu.selected) {
            case 0:
                if (it != players.end())
                    players.erase(it);
                break;
            case 1:
                if (it == players.end())
                    it = players
                             .emplace(key, Player(comboMenu.name, colors[key]))
                             .first;

                it->second.controller(false);
                it->second.setKey(Player::right,
                                  &window.keyboard.RIGHT.pressed);
                it->second.setKey(Player::left, &window.keyboard.LEFT.pressed);
                it->second.setKey(Player::up, &window.keyboard.UP.pressed);
                it->second.setKey(Player::down, &window.keyboard.DOWN.pressed);
                it->second.setKey(Player::action,
                                  &window.keyboard.SPACE.pressed);
                break;
            default:
                if (it == players.end())
                    it = players
                             .emplace(key, Player(comboMenu.name, colors[key]))
                             .first;

                it->second.controller(true);
                unsigned int controllerNumber = key - 1;

                if (GLFW::Window::joystickAxesCount[controllerNumber] < 2)
                    throw Blob::Exception("Controller don't have any joystick");

                it->second.setJoystickAxe(
                    Player::x,
                    &GLFW::Window::joystickAxes[controllerNumber][0]);
                it->second.setJoystickAxe(
                    Player::y,
                    &GLFW::Window::joystickAxes[controllerNumber][1]);
                it->second.setButton(
                    Player::action,
                    &GLFW::Window::joystickButtons[controllerNumber][0]);
                if (GLFW::Window::joystickButtonsCount[controllerNumber] >=
                    14) {
                    it->second.setButton(
                        Player::up,
                        &GLFW::Window::joystickButtons[controllerNumber][11]);
                    it->second.setButton(
                        Player::right,
                        &GLFW::Window::joystickButtons[controllerNumber][12]);
                    it->second.setButton(
                        Player::down,
                        &GLFW::Window::joystickButtons[controllerNumber][13]);
                    it->second.setButton(
                        Player::left,
                        &GLFW::Window::joystickButtons[controllerNumber][14]);
                }
                break;
            }
        }
    }
}
