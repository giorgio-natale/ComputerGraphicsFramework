//
// Created by drgio on 03/05/2023.
//

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <iostream>
#include "PerspectiveCamera.h"
#include "utils.h"
#include "GameEngine.h"

namespace fmwk {
    glm::mat4 PerspectiveCamera::getProjectionMatrix() {
        return glm::scale(I, glm::vec3(1,-1,1))
               * glm::perspective(_fovY, GameEngine::getInstance()->getAspectRatio(), _nearPlane, _farPlane);

    }

    PerspectiveCamera::PerspectiveCamera(float nearPlane, float farPlane, float fovY) : Camera(nearPlane), _farPlane(farPlane), _fovY(fovY) {}

    PerspectiveCamera::~PerspectiveCamera() = default;
} // fmwk