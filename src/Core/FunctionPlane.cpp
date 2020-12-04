#include <Core/FunctionPlane.hpp>

#include <Blob/Core/Exception.hpp>
#include <iostream>

using namespace Blob;
using namespace std;

FunctionPlaneCompact::FunctionPlaneCompact() {
    setBuffer(buffer, sizeof(Data));
    setArray<float>(3, Core::Shader::AttributeLocation::POSITION, 0);
    setArray<float>(3, Core::Shader::AttributeLocation::NORMAL, sizeof(Data::position));
}

void FunctionPlaneCompact::load(const Function2D &function2D, const Maths::Vec2<unsigned int> &numOfPlanes, const Maths::Vec2<float> &offset) {
    //    cout << "width : " << numOfPlanes << endl;
    //    cout << "Num of faces : " << numOfPlanes.x * numOfPlanes.y << endl;
    //    cout << "Num of triangles : " << numOfPlanes.x * numOfPlanes.y * 2 << endl;
    //    cout << "Number of points : " << (numOfPlanes.x + 1) * (numOfPlanes.y + 1) << endl;

    //    cout << "Allocating " << (numOfPlanes.x + 1) * (numOfPlanes.y + 1) * sizeof(Data) << " " << endl;
    data.resize((numOfPlanes.x + 1) * (numOfPlanes.y + 1));
    //    cout << "I got " << data.size() * sizeof(Data) << endl;

    multimap<float, array<unsigned int, 3>> orderedMap;

    if (numOfPlanes.x == 0 or numOfPlanes.y == 0)
        throw Blob::Core::Exception("numOfPlanes is null");

    //    cout << "Setting positions" << endl;
    for (unsigned int i = 0; i < numOfPlanes.x + 1; i++) {
        for (unsigned int j = 0; j < numOfPlanes.y + 1; j++) {
            size_t cursor = i * (numOfPlanes.y + 1) + j;
            float x1 = i - numOfPlanes.x / 2.f;
            float y1 = j - numOfPlanes.y / 2.f;
            data[cursor].position = {x1, y1, (float) function2D.get(x1 + offset.x, y1 + offset.y)};
        }
    }
    //    cout << "First pos : " << data[0].position << endl;
    //    cout << "Last pos : " << data[data.size() - 1].position << endl;

    for (unsigned int i = 0; i < numOfPlanes.x; i++) {
        for (unsigned int j = 0; j < numOfPlanes.y; j++) {
            unsigned int p0 = i * (numOfPlanes.y + 1) + j;
            unsigned int p1 = i * (numOfPlanes.y + 1) + j + 1;
            unsigned int p2 = (i + 1) * (numOfPlanes.y + 1) + j;
            unsigned int p3 = (i + 1) * (numOfPlanes.y + 1) + j + 1;

            if (data[p0].position.z > data[p1].position.z && data[p0].position.z > data[p2].position.z ||
                data[p3].position.z > data[p1].position.z && data[p3].position.z > data[p2].position.z) { // p0 ou p3 plus grand que p1 et p2
                float h1 = (data[p0].position.z + data[p1].position.z + data[p2].position.z) / 3.f;
                float h2 = (data[p1].position.z + data[p2].position.z + data[p3].position.z) / 3.f;

                orderedMap.emplace(h1, array<unsigned int, 3>{p0, p2, p1});
                orderedMap.emplace(h2, array<unsigned int, 3>{p1, p2, p3});
            } else { // p1 ou p2 le plus grand
                float h1 = (data[p0].position.z + data[p3].position.z + data[p1].position.z) / 3.f;
                float h2 = (data[p0].position.z + data[p3].position.z + data[p2].position.z) / 3.f;

                orderedMap.emplace(h1, array<unsigned int, 3>{p0, p3, p1});
                orderedMap.emplace(h2, array<unsigned int, 3>{p0, p2, p3});
            }
        }
    }
    //    cout << "I got " << orderedMap.size() << " triangles" << endl;

    indices.resize(orderedMap.size() * 3);
    //    cout << "I got " << indices.size() << " indices" << endl;

    size_t cursor = 0;
    for (auto &[h, d] : orderedMap) {
        auto i0 = d[0];
        auto i1 = d[1];
        auto i2 = d[2];
        Maths::Vec3<float> A = data[i0].position;
        Maths::Vec3<float> B = data[i1].position;
        Maths::Vec3<float> C = data[i2].position;
        Maths::Vec3<float> N = (B - A).cross(C - A);

        data[i0].nomal += N;
        data[i1].nomal += N;
        data[i2].nomal += N;

        indices[cursor + 0] = i0;
        indices[cursor + 1] = i1;
        indices[cursor + 2] = i2;

        if (heightPosition.find(h) == heightPosition.end())
            heightPosition[h] = cursor;

        cursor += 3;
    }

    for (auto &d : data)
        d.nomal = d.nomal.normalize();
}

void FunctionPlaneCompact::set() {
    heightPosition.clear();
    if (data.empty())
        return;
    buffer.setData(data);
    data.clear();
}

const Blob::Core::Primitive &FunctionPlaneCompact::getPrimitive(const Blob::Core::Material &material) {
    const auto &ro = renderOptions.emplace_back(indices.data(), indices.size());
    const auto &p = primitives.emplace_back(this, &material, &ro);
    return p;
}

FunctionPlane::FunctionPlane(const Core::Material &material) : primitive(&vertexArrayObject, &material, &renderOptions) {
    vertexArrayObject.setBuffer(buffer, sizeof(Data));
    vertexArrayObject.setArray<float>(3, Core::Shader::AttributeLocation::POSITION, 0);
    vertexArrayObject.setArray<float>(3, Core::Shader::AttributeLocation::NORMAL, sizeof(Data::position));
}

void FunctionPlane::load(const Function2D &function2D, const Maths::Vec2<unsigned int> &numOfPlanes, const Blob::Maths::Vec2<float> &offset,
                         const Blob::Maths::Vec2<float> &planeSize) {
    data.resize(numOfPlanes.x * numOfPlanes.y * 6);

    if (numOfPlanes.x == 0 or numOfPlanes.y == 0)
        throw Blob::Core::Exception("numOfPlanes is null");

    size_t cursor = 0;
    for (unsigned int i = 0; i < numOfPlanes.x; i++) {
        for (unsigned int j = 0; j < numOfPlanes.y; j++) {
            float x1 = planeSize.x * (static_cast<float>(i) - numOfPlanes.x / 2.f);
            float x2 = planeSize.x * (static_cast<float>(i) + 1.f - numOfPlanes.x / 2.f);
            float y1 = planeSize.y * (static_cast<float>(j) - numOfPlanes.y / 2.f);
            float y2 = planeSize.y * (static_cast<float>(j) + 1.f - numOfPlanes.y / 2.f);

            // TODO: Use  GL_UNSIGNED_INT_2_10_10_10_REV
            if (function2D.exist(x1 + offset.x, y1 + offset.y) || function2D.exist(x1 + offset.x, y2 + offset.y) ||
                function2D.exist(x2 + offset.x, y1 + offset.y) || function2D.exist(x2 + offset.x, y2 + offset.y)) {
                Maths::Vec3<float> A = {x1, y1, (float) function2D.get(x1 + offset.x, y1 + offset.y)};
                Maths::Vec3<float> B = {x1, y2, (float) function2D.get(x1 + offset.x, y2 + offset.y)};
                Maths::Vec3<float> C = {x2, y1, (float) function2D.get(x2 + offset.x, y1 + offset.y)};
                Maths::Vec3<float> D = {x2, y2, (float) function2D.get(x2 + offset.x, y2 + offset.y)};

                // TODO: Use with GL_UNSIGNED_INT_10F_11F_11F_REV
                Maths::Vec3<float> N1 = (C - A).cross(B - A).normalize();
                Maths::Vec3<float> N2 = (C - B).cross(D - B).normalize();
                data[cursor + 0] = {A, N1};
                data[cursor + 1] = {C, N1};
                data[cursor + 2] = {B, N1};
                data[cursor + 3] = {B, N2};
                data[cursor + 4] = {C, N2};
                data[cursor + 5] = {D, N2};
                cursor += 6;
            }
        }
    }

    //    cout << "NoisePlane : " << data.size() << endl;

    if (cursor != 0)
        data.resize(cursor);
}
void FunctionPlane::load(const Function3D &function3D, const Maths::Vec3<unsigned int> &numOfPlanes, const Blob::Maths::Vec3<float> &offset,
                         const Blob::Maths::Vec3<float> &planeSize) {
    data.resize(numOfPlanes.x * numOfPlanes.y * numOfPlanes.z * 6);

    if (numOfPlanes.x == 0 or numOfPlanes.y == 0 or numOfPlanes.z == 0)
        throw Blob::Core::Exception("numOfPlanes is null");

    size_t cursor = 0;
    for (unsigned int i = 0; i < numOfPlanes.x; i++) {
        for (unsigned int j = 0; j < numOfPlanes.y; j++) {
            for (unsigned int k = 0; k < numOfPlanes.z; k++) {
                float x1 = planeSize.x * (static_cast<float>(i) - numOfPlanes.x / 2.f);
                float x2 = planeSize.x * (static_cast<float>(i) + 1.f - numOfPlanes.x / 2.f);
                float y1 = planeSize.y * (static_cast<float>(j) - numOfPlanes.y / 2.f);
                float y2 = planeSize.y * (static_cast<float>(j) + 1.f - numOfPlanes.y / 2.f);
                float z1 = planeSize.z * (static_cast<float>(k) - numOfPlanes.z / 2.f);
                float z2 = planeSize.z * (static_cast<float>(k) + 1.f - numOfPlanes.z / 2.f);

                // TODO: Use  GL_UNSIGNED_INT_2_10_10_10_REV
                if (function3D.exist(x1 + offset.x, y1 + offset.y, z1 + offset.z) || function3D.exist(x1 + offset.x, y2 + offset.y, z2 + offset.z) ||
                    function3D.exist(x2 + offset.x, y1 + offset.y, z1 + offset.z) || function3D.exist(x2 + offset.x, y2 + offset.y, z2 + offset.z)) {
                    Maths::Vec3<float> A = {x1, y1, (float) function3D.get(x1 + offset.x, y1 + offset.y, z1 + offset.z)};
                    Maths::Vec3<float> B = {x1, y2, (float) function3D.get(x1 + offset.x, y2 + offset.y, z2 + offset.z)};
                    Maths::Vec3<float> C = {x2, y1, (float) function3D.get(x2 + offset.x, y1 + offset.y, z1 + offset.z)};
                    Maths::Vec3<float> D = {x2, y2, (float) function3D.get(x2 + offset.x, y2 + offset.y, z2 + offset.z)};

                    // TODO: Use with GL_UNSIGNED_INT_10F_11F_11F_REV
                    Maths::Vec3<float> N1 = (C - A).cross(B - A).normalize();
                    Maths::Vec3<float> N2 = (C - B).cross(D - B).normalize();
                    data[cursor + 0] = {A, N1};
                    data[cursor + 1] = {C, N1};
                    data[cursor + 2] = {B, N1};
                    data[cursor + 3] = {B, N2};
                    data[cursor + 4] = {C, N2};
                    data[cursor + 5] = {D, N2};
                    cursor += 6;
                }
            }
        }
    }

    //    cout << "NoisePlane : " << data.size() << endl;

    if (cursor != 0)
        data.resize(cursor);
}

void FunctionPlane::set() {
    if (data.empty())
        return;
    buffer.setData(data);
    renderOptions.setArray(data.size());
    data.clear();

    addPrimitive(primitive);
}
