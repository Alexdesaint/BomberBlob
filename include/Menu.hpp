#pragma once

#include <Blob/GL/Graphic.hpp>

#include <Player.hpp>

#include <BomberBlob/Textures.hpp>

#include <map>

class Menu {
private:
	Textures textures;

	Blob::GL::Graphic &window;
	std::map<int, Player> players;

	void playerSelection();

public:
	explicit Menu(Blob::GL::Graphic &window);
};