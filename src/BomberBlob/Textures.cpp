#include <BomberBlob/Textures.hpp>

Textures::Textures() :
        groundTex("data/Grass.bmp", true),
        boxTex("data/Box.bmp", true),
        indestructibleBoxTex("data/IndestructibleBox.bmp", true),
        extraBombTex("data/ExtraBomb.bmp", true),
        extraPowerTex("data/ExtraPower.bmp", true),
        extraSpeedTex("data/ExtraSpeed.bmp", true),
        bombTex("data/Bomb.bmp", true),
        explosionTex("data/Explosion.bmp", true),

        brown(Blob::Color::Brown),
        gray(Blob::Color::Gray),
        red(Blob::Color::Red),
        orange(Blob::Color::Orange),
        yellow(Blob::Color::Yellow),
        green(Blob::Color::Green),
        black(Blob::Color::Black),

        groundMat(groundTex),
        boxMat(boxTex),
        indestructibleBoxMat(indestructibleBoxTex),
        extraBombMat(extraBombTex),
        extraPowerMat(extraPowerTex),
        extraSpeedMat(extraSpeedTex),
        bombMat(bombTex),
        explosionMat(explosionTex),

        ground(groundMat),
        box(boxMat),
        indestructibleBox(indestructibleBoxMat),
        extraBomb(extraBombMat),
        extraPower(extraPowerMat),
        extraSpeed(extraSpeedMat),
        bomb(bombMat),
        explosion(explosionMat),
        player{
                Blob::Shapes::Cube(orange),
                Blob::Shapes::Cube(red),
                Blob::Shapes::Cube(yellow),
                Blob::Shapes::Cube(black)} {
    boxMat.texScale[0] = 4.f;
    boxMat.texScale[1] = 4.f;

    indestructibleBoxMat.texScale[0] = 4.f;
    indestructibleBoxMat.texScale[1] = 4.f;

    extraBombMat.texScale[0] = 4.f;
    extraBombMat.texScale[1] = 4.f;

    extraPowerMat.texScale[0] = 4.f;
    extraPowerMat.texScale[1] = 4.f;

    extraSpeedMat.texScale[0] = 4.f;
    extraSpeedMat.texScale[1] = 4.f;

    explosionMat.texScale[0] = 4.f;
    explosionMat.texScale[1] = 4.f;
}
