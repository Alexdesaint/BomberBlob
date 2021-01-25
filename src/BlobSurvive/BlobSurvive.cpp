#include <BlobSurvive/BlobSurvive.hpp>
#include <box2d/box2d.h>
#include <numbers>

#include <Core/DynamicCollider.hpp>

#include <BlobSurvive/Tree.hpp>
#include <Core/FunctionPlane.hpp>
#include <Core/StaticCube.hpp>
#include <deque>
#include <future>
#include <imgui.h>
#include <random>

using namespace Blob;
using namespace std;

BlobSurvive::BlobSurvive(Blob::Core::Window &window, std::map<int, Player> &players)
    : Game(window, players, {{0, 1000, 1000}, {0, 200, 0}, {0, 0, 1}}), waterFunction(groundFunction), survivor(world, {0, 0}, groundFunction) {}

Terrain::Terrain()
    : underWaterMat(Color::LightYellow), sandMat(Color::SandyBrown), grassMat(Color::Green), mountainsMat(Color::DimGray),
      waterMat(Color::RGBA(0.09, 0.37, 0.6, 0.8f)), rockMat(Color::DimGray) {}

Terrain::Tile::Tile(const Terrain &terrain, const GroundFunction &groundFunction, const WaterFunction &waterFunction,
                    const Maths::Vec2<float> &tileCenter)
    : Core::Shape(groundWater), terrain(terrain), water(terrain.waterMat),
      loadingThread(&Terrain::Tile::load, this, groundFunction, waterFunction, tileCenter) {}

Terrain::Tile::~Tile() {
    if (loadingThread.joinable())
        loadingThread.join();
}

void Terrain::Tile::load(const GroundFunction &groundFunction, const WaterFunction &waterFunction, const Maths::Vec2<float> &tileCenter) {
    setPosition(tileCenter);
    water.load(waterFunction, tilesSize / 10, tileCenter, {10, 10});
    groundWater.addTransparentPrimitive(water.primitive);

    ground.load(groundFunction, tilesSize, tileCenter);
    const Blob::Core::Material *material[4]{&terrain.underWaterMat, &terrain.sandMat, &terrain.grassMat, &terrain.mountainsMat};
    float separator[3]{1.f, 1.5f, 20.f};
    auto &p = ground.getPrimitives<4>(material, separator);
    for (auto &q : p)
        groundWater.addPrimitive(q);
    loaded = true;
}

bool Terrain::Tile::ready() {
    if (_ready)
        return true;
    if (loaded) {
        loadingThread.join();
        water.set();
        ground.set();
        _ready = true;
    }
    return _ready;
}

void Terrain::generate(b2World &world, const GroundFunction &groundFunction, const WaterFunction &waterFunction) {
    loaded = false;
    removeAll();
    tiles.clear();

    Maths::Vec2<float> n = (numOfTiles.cast<float>() - 1) / -2.f;
    Maths::Vec2<unsigned int> tileNumber{};
    // tiles.reserve(numOfTiles.x * numOfTiles.y);
    for (tileNumber.x = 0; tileNumber.x < numOfTiles.x; tileNumber.x++)
        for (tileNumber.y = 0; tileNumber.y < numOfTiles.y; tileNumber.y++)
            addShape(tiles.emplace_back(*this, groundFunction, waterFunction, (tileNumber.cast<float>() + n) * tilesSize.cast<float>()));

    std::default_random_engine e1(r());

    std::uniform_real_distribution<float> positionDist(-(tilesSize.x * tileNumber.x / 2.f), tilesSize.x * tileNumber.x / 2.f);
    std::uniform_int_distribution<int> sizeDist(10, 20);
    for (int k = 0; k < 5000; k++) {
        float x = positionDist(e1);
        float y = positionDist(e1);
        float z = groundFunction.get(x, y);
        if (z > 1 && z < 10)
            addShape(staticCubes.emplace_back(world, Maths::Vec3{x, y, z}, sizeDist(e1) / 10.f, rockMat, 1));
    }

    for (int k = 0; k < 5000; k++) {
        float x = positionDist(e1);
        float y = positionDist(e1);
        float z = groundFunction.get(x, y);
        if (z > 1 && z < 10)
            addShape(trees.emplace_back(world, Maths::Vec3{x, y, z}));
    }
}

bool Terrain::finished() {
    if (loaded)
        return true;

    for (auto &t : tiles)
        if (!t.ready())
            return false;

    loaded = true;
    return true;
}

void BlobSurvive::keyboardUpdate(const Blob::Core::Key &key) {
    if (key.id == Blob::GLFW::Keys::M)
        mouseEnabled = !mouseEnabled;
    if (mouseEnabled)
        window.enableMouseCursor();
    else
        window.disableMouseCursor();
    if (key.id == Blob::GLFW::Keys::C)
        worldCamera = !worldCamera;
    if (worldCamera)
        window.setCamera(camera);
    else
        window.setCamera(survivor.camera);
}

void BlobSurvive::run() {
    Core::Scene scene;
    window.disableMouseCursor();
    window.setCamera(survivor.camera);

    // Body
    scene.addShape(survivor);

    // ?
    int32 velocityIterations = 6;
    int32 positionIterations = 2;

    float timeStep = 0;

    auto &io = ImGui::GetIO();
    while (window.isOpen()) {
        world.Step(timeStep, velocityIterations, positionIterations);

        DynamicCollider::updateAll();
        window.draw(scene);
        window.draw(terrain);
        timeStep = window.display();
    }
}

void BlobSurvive::load() {
    float cameraAngle = PI / 4;
    window.setAngle(cameraAngle);
    window.setRange(1, 10000);

    Materials::PBR::light.position = {4000, 4000, 4000};
    Materials::PBR::light.power = 100000000.f;
    terrain.generate(world, groundFunction, waterFunction);
}

bool BlobSurvive::gameReady() {
    window.draw(terrain);
    return terrain.loaded;
}

bool BlobSurvive::loadReady() {
    return terrain.finished();
}

void BlobSurvive::settings() {
    groundFunction.window();
}

double GroundFunction::get(double x, double y) const {
    double d = x * x + y * y;
    double a = atan2(y, x);
    double an1 = (1 - sin(a / 2) * sin(a / 2)) * perlinNoise.noise(perlinZ, a, perlinZ);
    a = atan2(-y, -x);
    double an2 = (1 - sin(a / 2) * sin(a / 2)) * perlinNoise.noise(a, perlinZ, perlinZ);
    double an = (an1 + an2) * (beachBorder - 50);
    double bb = beachBorder + an;
    double bb2 = bb * bb;

    double hills = perlinNoise.noise(x / 20, y / 20, perlinZ);
    double largeHills = perlinNoise.noise(x / 20, y / 20, perlinZ);
    double circleHills = 2 * (0.5 - abs(0.5 - perlinNoise.noise(x / 20, y / 20, perlinZ)));

    double f0 = -atan((d - pow(beachBorder + an, 2)) / 500) / numbers::pi + 0.5;                                              // beach border
    double f1 = (-atan((d - pow(beachBorder - 20 + an, 2)) / 500) / numbers::pi + 0.5) * abs(hills);                          // 1st ground
    double f2 = -atan(d / 500 - 1) / numbers::pi + atan(d / 500) / numbers::pi;                                               // Volcano
    double f3 = (-atan((d - pow(beachBorder + 80 + an, 2)) / 500) + atan((d - pow(beachBorder + 30 + an, 2)) / 500)) * hills; // Exterior border
    double f4 = (-d + bb2) / bb2;
    double f50 = (d < (bb2 - 800)) ? hills : -1;
    double f5 = ceil(f4 + f50)+0.5;
    double f6 = 0;
    double f7 = 0;

    return offset + f0 * fGain[0] + f1 * fGain[1] + f2 * fGain[2] + f3 * fGain[3] + f4 * fGain[4] + f5 * fGain[5] + f6 * fGain[6] + f7 * fGain[7];
}

void GroundFunction::window() {
    ImGui::SliderFloat("Perlin z", &perlinZ, 0, 255);
    ImGui::SliderInt("offset", &offset, -100, 100);
    ImGui::SliderFloat("beachBorder", &beachBorder, 100, 1000);
    for (int i = 0; i < fGain.size(); i++)
        ImGui::SliderInt((to_string(i) + " Gain").c_str(), &fGain[i], -50, 50);
}
