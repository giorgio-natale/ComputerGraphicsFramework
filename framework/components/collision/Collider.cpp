//
// Created by drgio on 04/08/2023.
//

#include "Collider.h"

#include <utility>

namespace fmwk {
    bool Collider::isProvisioned() const {
        return _alreadyProvisioned;
    }

    Collider::Collider(float radius, std::string  tag) : Component("Collider"),
                                                                                        _radius(radius),
                                                                                        _alreadyProvisioned(false),
                                                                                        _tag(std::move(tag)) {}

    void Collider::provision() {
        _alreadyProvisioned = true;
    }

    float Collider::getRadius() const {
        return _radius;
    }

    const std::string &Collider::getTag() const {
        return _tag;
    }
} // fmwk