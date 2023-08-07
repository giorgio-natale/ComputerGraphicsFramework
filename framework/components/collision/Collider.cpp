//
// Created by drgio on 04/08/2023.
//

#include "Collider.h"

namespace fmwk {
    bool Collider::isProvisioned() const {
        return _alreadyProvisioned;
    }

    Collider::Collider(const std::string &name, float radius, std::string const &tag) : Component(name),
                                                                                        _radius(radius),
                                                                                        _alreadyProvisioned(false),
                                                                                        _tag(tag) {}

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