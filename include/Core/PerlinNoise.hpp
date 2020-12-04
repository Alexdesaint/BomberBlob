#pragma once
#include <vector>

class Function2D {
public:
    virtual double get(double x, double y) const = 0;

    virtual bool exist(double x, double y) const { return true; };
};

class Function3D {
public:
    virtual double get(double x, double y, double z) const = 0;

    virtual bool exist(double x, double y, double z) const { return true; };
};

class PerlinNoise : public Function2D, public Function3D { /// Source : https://cs.nyu.edu/~perlin/noise/
private:
    // The permutation vector
    std::vector<int> p;

    static double fade(double t) { return t * t * t * (t * (t * 6 - 15) + 10); }
    static double lerp(double t, double a, double b) { return a + t * (b - a); }
    static double grad(int hash, double x, double y, double z) {
        /// CONVERT LO 4 BITS OF HASH CODE INTO 12 GRADIENT DIRECTIONS.
        int h = hash & 15;
        double u = h < 8 ? x : y, v = h < 4 ? y : h == 12 || h == 14 ? x : z;
        return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
    }

public:
    // Initialize with the reference values for the permutation vector
    PerlinNoise();
    // Generate a new permutation vector based on the value of seed
    explicit PerlinNoise(unsigned int seed);
    // Get a noise value, for 2D images z can have any value
    double noise(double x, double y, double z) const;

    [[nodiscard]] double get(double x, double y) const final { return noise(x, y, 0.5); }

    [[nodiscard]] double get(double x, double y, double z) const final { return noise(x, y, z); }
};
