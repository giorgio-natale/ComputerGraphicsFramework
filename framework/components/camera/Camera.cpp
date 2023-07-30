//
// Created by drgio on 01/05/2023.
//

#include "Camera.h"
#include "../../Entity.h"
#include "../transform/Transform.h"

namespace fmwk {
    Camera::Camera(float nearPlane) : Component("Camera"), _nearPlane(nearPlane) {}

    glm::mat4 Camera::getViewMatrix() const {
        auto& transform = _parentEntity->getTransform();
        return transform.getInverseWorldMatrix();
    }

    Camera::~Camera() = default;
} // fmwk