#pragma once

#include <Blob/GL/Texture.hpp>
#include <Blob/Shapes.hpp>

class Textures {
public:
    Blob::SingleColorMaterial brown, gray, red, orange, yellow, green, black, fire;

    Blob::GL::Texture groundTex,
            boxTex,
            indestructibleBoxTex,
            extraBombTex,
            extraPowerTex,
            extraSpeedTex,
            bombTex,
            explosionTex;

    Blob::SingleTextureMaterial groundMat,
            boxMat,
            indestructibleBoxMat,
            extraBombMat,
            extraPowerMat,
            extraSpeedMat,
            bombMat,
            explosionMat;

    Blob::Shapes::Plane ground;
    Blob::Shapes::Cube box,
            indestructibleBox,
            extraBomb,
            extraPower,
            extraSpeed,
            bomb,
            explosion,
            player[4];

    Textures();
};