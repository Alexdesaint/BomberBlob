#pragma once

#include <Blob/Window.hpp>

#include <Player.hpp>

#include <map>

class Game {
protected:
	Blob::Window &window;
	std::map<int, Player> &players;

	explicit Game(Blob::Window &window, std::map<int, Player> &players) : window(window), players(players) {}
};
