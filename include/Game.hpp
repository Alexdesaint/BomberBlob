#pragma once

#include <Blob/Core/Window.hpp>

#include <Player.hpp>

#include <map>
#include <utility>

class Game {
protected:
    Blob::Window &window;
    std::map<int, Player> &players;

    explicit Game(Blob::Window &window,
                  std::map<int, Player> &players,
                  Blob::Camera camera) :
        window(window), players(players), camera(std::move(camera)) {}

public:
    Blob::Camera camera;

    virtual void run() = 0;

    virtual void load(){};
    virtual void settings(){};
    virtual bool loadReady() { return false; };
    virtual bool gameReady() { return true; };

    virtual ~Game() = default;
};
