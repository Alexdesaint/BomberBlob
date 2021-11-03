#pragma once

#include <Blob/Core/Texture.hpp>
#include <Blob/Materials.hpp>
#include <Core/StaticCube.hpp>
#include <box2d/box2d.h>

class Bonus : public StaticCube {
private:
    bool destroyed = false;

    Blob::TextureAsset<"data/ExtraBomb.bmp">::Instance extraBombTex;
    Blob::TextureAsset<"data/ExtraPower.bmp">::Instance extraPowerTex;
    Blob::TextureAsset<"data/ExtraSpeed.bmp">::Instance extraSpeedTex;

    Blob::Materials::SingleTexture extraBombMat{*extraBombTex, {4, 4}};
    Blob::Materials::SingleTexture extraPowerMat{*extraPowerTex, {4, 4}};
    Blob::Materials::SingleTexture extraSpeedMat{*extraSpeedTex, {4, 4}};

public:
    Bonus(const Blob::Vec2<float> &pos, b2World &world, float width = 0.4f);

    void hit(Collider *c) final;

    bool update() const;
};
