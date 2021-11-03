#pragma once

#include <Blob/Core/Window.hpp>
#include <Player.hpp>
//#include <BomberBlob/Textures.hpp>
#include <map>

class Menu {
private:
    // Textures textures;

    Blob::Window &window;
    std::map<int, Player> players;

    void playerSelection();

public:
    explicit Menu(Blob::Window &window);
};