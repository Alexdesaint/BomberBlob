#pragma once

#include <mutex>

class Collider {
public:
    unsigned int id = 0;

    std::mutex mutex;

    Collider() = default;
    Collider(const Collider &) = delete;
    Collider(Collider &&) = delete;
    explicit Collider(unsigned int id) : id(id) {}

    virtual void hit(Collider *) {}
};