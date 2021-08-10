#pragma once

#include <BlobSurvive/Survivor.hpp>
#include <BlobSurvive/Tree.hpp>
#include <Core/FunctionPlane.hpp>
#include <Core/PerlinNoise.hpp>
#include <Core/StaticCube.hpp>
#include <Game.hpp>
#include <deque>
#include <future>
#include <random>

class GroundFunction : public Function2D {
private:
    PerlinNoise perlinNoise;

    float perlinZ = 0.5;

    int offset = -10;
    float beachBorder = 600;
    std::vector<int> fGain{0, 0, 0, 20, 3, 3, 10, 0};

public:
    explicit GroundFunction(unsigned int preset = 0) {
        switch (preset) {
        case 1: // islands
            offset = -10;
            beachBorder = 600;
            fGain = {0, 0, 0, 20, 3, 3, 10, 0};
            break;
        case 2: // just forests
            offset = 5;
            beachBorder = 600;
            fGain = {0, 0, 0, 1, 1, 1, 1, 15};
            break;
        case 3: // tiles islands
            offset = 0;
            beachBorder = 600;
            fGain = {0, 0, 0, 0, 0, 2, 1, 1};
            break;
        default:
            offset = 0;
            beachBorder = 600;
            fGain = {0, 1, 200, 3, 10, 0, 0, 0};
            break;
        }
    }

    double get(double x, double y) const final;

    void window();
};

class WaterFunction : public Function2D {
private:
    Function2D &ground;

public:
    PerlinNoise perlinNoise;
    explicit WaterFunction(Function2D &ground) : ground(ground) {}

    double get(double x, double y) const final { return perlinNoise.noise(x, y, 0.5); }

    bool exist(double x, double y) const final { return ground.get(x, y) < 0; }
};

class Terrain : public Blob::Scene {
private:
    struct Tile : Blob::Shape {
    private:
        const Terrain &terrain;

        FunctionPlaneCompact ground;
        FunctionPlane water;
        Blob::Mesh groundWater;

        std::thread loadingThread;
        std::atomic<bool> loaded = false;
        bool _ready = false;

    public:
        Tile(const Terrain &terrain, const GroundFunction &groundFunction, const WaterFunction &waterFunction, const Blob::Vec2<float> &tileCenter);
        ~Tile();
        void load(const GroundFunction &groundFunction, const WaterFunction &waterFunction, const Blob::Vec2<float> &tileCenter);

        bool ready();
    };

    Blob::Materials::PBRSingleColor underWaterMat;
    Blob::Materials::PBRSingleColor sandMat;
    Blob::Materials::PBRSingleColor grassMat;
    Blob::Materials::PBRSingleColor mountainsMat;

    Blob::Materials::PBRWater waterMat;

    Blob::Materials::PBRSingleColor rockMat;

    std::random_device r;
    std::list<Tile> tiles;
    std::deque<StaticCube> staticCubes;
    std::deque<Tree> trees;

    const Blob::Vec2<unsigned int> numOfTiles{6, 4};
    inline static const Blob::Vec2<unsigned int> tilesSize{500, 500}; /// Must be a multiple of 10

public:
    bool loaded = false;

    Terrain();

    bool finished();

    void generate(b2World &world, const GroundFunction &groundFunction, const WaterFunction &waterFunction);
};

class BlobSurvive : public Game, private Blob::KeyboardEvents {
private:
    Blob::AssetManager assetManager;
    b2World world{{0, 0}};
    Terrain terrain;
    Survivor survivor;

    // Noise
    GroundFunction groundFunction;
    WaterFunction waterFunction;

    bool mouseEnabled = false, worldCamera = false;

    void keyboardUpdate(const Blob::Key &key) final;

public:
    BlobSurvive(Blob::Window &window, std::map<int, Player> &players);

    void run() final;

    void load() final;

    void settings() final;

    bool loadReady() final;

    bool gameReady() final;
};
