#pragma once

#include <utility>

class Player {
public:
	enum Actions {
		up = 0,
		down,
		right,
		left,
		action,
		numOfActions
	};

	enum JoystickAxe {
		x = 0,
		y,
		numOfAxes
	};

	const unsigned char* buttons[Actions::numOfActions] = {nullptr};

	const float *joystickAxes[numOfAxes] = {nullptr};

	const bool* keys[Actions::numOfActions] = {nullptr};

	std::string name;

	int score = 0;

	bool control = false;

        Blob::Color::RGB color;

	Player(std::string name, Blob::Color::RGB color) : name(std::move(name)), color(color) {}

	void controller(bool c) {
		control = c;
	}

	void setJoystickAxe(JoystickAxe a, const float *joy) {
		joystickAxes[a] = joy;
	}

	void setKey(Actions a, const bool *key) {
		keys[a] = key;
	}

	void setButton(Actions a, const unsigned char *button) {
		buttons[a] = button;
	}

	const std::string &getName() const {
		return name;
	}

	void setName(const std::string &n) {
		name = n;
	}
};
