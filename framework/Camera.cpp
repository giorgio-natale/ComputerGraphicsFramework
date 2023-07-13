//
// Created by drgio on 01/05/2023.
//

#include "Camera.h"
#include "Entity.h"
#include "Transform.h"

namespace fmwk {
    Camera::Camera(const std::string &name, float nearPlane) : Component(name), _nearPlane(nearPlane) {}

    glm::mat4 Camera::getViewMatrix() const {
        auto& transform = _parentEntity->getTransform();
        return transform.getInverseWorldMatrix();
    }

    Camera::~Camera() = default;
} // fmwk