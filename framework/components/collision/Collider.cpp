//
// Created by drgio on 04/08/2023.
//

#include "Collider.h"

namespace fmwk {
    bool Collider::isProvisioned() const {
        return _alreadyProvisioned;
    }

    Collider::Collider(const std::string &name, float radius) : Component(name), _radius(radius), _alreadyProvisioned(false){}

    void Collider::provision() {
        _alreadyProvisioned = true;
    }

    float Collider::getRadius() const {
        return _radius;
    }
} // fmwk