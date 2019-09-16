#pragma once

#include <Blob/GL/Graphic.hpp>

#include <Player.hpp>

#include <map>

class Game {
protected:
	Blob::GL::Graphic &window;
	std::map<int, Player> &players;

	explicit Game(Blob::GL::Graphic &window, std::map<int, Player> &players) : window(window), players(players) {}
};
