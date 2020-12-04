#pragma once

#include <Blob/AssetManager.hpp>
#include <Blob/Color.hpp>

class Textures : Blob::AssetManager {
public:
    Blob::Materials::SingleColor defaultMat;

    Blob::Core::Texture groundTex, boxTex, indestructibleBoxTex, extraBombTex, extraPowerTex, extraSpeedTex, bombTex, explosionTex;

    Blob::Materials::SingleTexture groundMat, boxMat, indestructibleBoxMat, extraBombMat, extraPowerMat, extraSpeedMat, bombMat, explosionMat;

    Textures();
};