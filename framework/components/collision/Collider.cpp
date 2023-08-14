//
// Created by drgio on 04/08/2023.
//

#include "Collider.h"

#include <utility>

namespace fmwk {
    bool Collider::isProvisioned() const {
        return _alreadyProvisioned;
    }

    Collider::Collider(float radius, std::string tag, glm::vec3 centerOffset) : Component("Collider"),
                                                                                _radius(radius),
                                                                                _alreadyProvisioned(false),
                                                                                _tag(std::move(tag)),
                                                                                _centerOffset(centerOffset) {}

    void Collider::provision() {
        _alreadyProvisioned = true;
    }

    float Collider::getRadius() const {
        return _radius;
    }

    const std::string &Collider::getTag() const {
        return _tag;
    }

    const glm::vec3 &Collider::getCenterOffset() const {
        return _centerOffset;
    }
} // fmwk