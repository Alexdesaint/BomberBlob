#pragma once

#include <Blob/Core/Buffer.hpp>
#include <Blob/Core/Mesh.hpp>
#include <Blob/Materials.hpp>
#include <Core/PerlinNoise.hpp>
#include <map>

class FunctionPlaneCompact : public Blob::GL::VertexArrayObject {
    struct Data {
        Blob::Maths::Vec3<float> position;
        Blob::Maths::Vec3<float> nomal;
    };

private:
    std::vector<Data> data;

    Blob::Core::Buffer buffer;
    std::vector<unsigned int> indices;

    std::list<Blob::Core::RenderOptions> renderOptions;
    std::list<Blob::Core::Primitive> primitives;
    std::map<float, size_t> heightPosition;

public:
    FunctionPlaneCompact();
    void load(const Function2D &function2D, const Blob::Maths::Vec2<unsigned int> &numOfPlanes, const Blob::Maths::Vec2<float> &offset = {0, 0});

    const Blob::Core::Primitive &getPrimitive(const Blob::Core::Material &material);

    template<size_t N>
    const std::list<Blob::Core::Primitive> &getPrimitives(const Blob::Core::Material *material[N], float separator[N - 1]) {

        size_t pos = 0;
        for (size_t i = 0; i < N - 1; i++) {
            if (separator[i] >= heightPosition.begin()->first) { // if the separator is superior to the lower height
                auto it = heightPosition.lower_bound(separator[i]);
                if (it != heightPosition.end()) { // if the separator is in the data
                    const auto &ro = renderOptions.emplace_back(indices.data() + pos, it->second - pos);
                    pos = it->second;
                    primitives.emplace_back(this, material[i], &ro);
                } else { // if the separator superior to the hiest value
                    const auto &ro = renderOptions.emplace_back(indices.data() + pos, indices.size() - pos);
                    primitives.emplace_back(this, material[i], &ro);
                    return primitives;
                }
            }
        }

        const auto &ro = renderOptions.emplace_back(indices.data() + pos, indices.size() - pos);
        primitives.emplace_back(this, material[N - 1], &ro);
        return primitives;
    }

    void set();
};

class FunctionPlane : public Blob::Core::Mesh {
    struct Data {
        Blob::Maths::Vec3<float> position;
        Blob::Maths::Vec3<float> nomal;
    };

private:
    std::vector<Data> data;

    Blob::Core::Buffer buffer;
    Blob::GL::VertexArrayObject vertexArrayObject;
    Blob::Core::RenderOptions renderOptions;

public:
    const Blob::Core::Primitive primitive;

    explicit FunctionPlane(const Blob::Core::Material &material);
    void load(const Function2D &function2D, const Blob::Maths::Vec2<unsigned int> &numOfPlanes, const Blob::Maths::Vec2<float> &offset = {0, 0},
              const Blob::Maths::Vec2<float> &planeSize = {1, 1});
    void load(const Function3D &function3D, const Blob::Maths::Vec3<unsigned int> &numOfPlanes, const Blob::Maths::Vec3<float> &offset = {0, 0, 0},
              const Blob::Maths::Vec3<float> &planeSize = {1, 1, 1});
    void set();
};
