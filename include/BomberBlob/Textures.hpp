#pragma once

#include <Blob/GL/Texture.hpp>

class Textures {
public:
	Blob::GL::Texture ground,
			box,
			indestructibleBox,
			extraBomb,
			extraPower,
			extraSpeed,
			bomb,
			explosion,
			player[4];

	Textures();
};