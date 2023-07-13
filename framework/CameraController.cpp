//
// Created by drgio on 04/05/2023.
//

#include <memory>
#include <iostream>
#include "CameraController.h"
#include "GameEngine.h"
#include "Entity.h"
#include "utils.h"

namespace fmwk {
    CameraController::CameraController(const std::string &name, const Transform &characterTransform,
                                       float angularSpeed, float cameraDistance, float cameraHeight) : Component(name),
                                                                   _characterTransform(characterTransform),
                                                                   _angularSpeed(angularSpeed),
                                                                   _cameraDistance(cameraDistance),
                                                                   _cameraHeight(cameraHeight){}

    void CameraController::postUpdate() {
        float deltaT;
        auto m = glm::vec3(0.0f), r = glm::vec3(0.0f);
        bool fire = false;
        auto& cameraTransform =_parentEntity->getTransform();;
        fmwk::GameEngine::getInstance() -> handleInputs(deltaT, m, r, fire);
        _rotationAboutCenter = _rotationAboutCenter + EulerVector{r.x * _angularSpeed * deltaT,r.y * _angularSpeed * deltaT,0};
        _rotationAboutCenter = EulerVector{glm::clamp(_rotationAboutCenter[0], glm::radians(-8.75f), glm::radians(60.0f)), _rotationAboutCenter[1] , _rotationAboutCenter[2]};

        auto cameraBasePosition = _characterTransform.getPosition() + glm::vec3(0, _cameraHeight, _cameraDistance);
        auto cameraPosition = glm::vec3 (rotationAroundPoint(
                _characterTransform.getPosition(),
                _rotationAboutCenter.getVector()[1],
                _rotationAboutCenter.getVector()[0]) * glm::vec4(cameraBasePosition, 1));

        cameraTransform.setPosition(cameraPosition);
        auto lookingDirection = glm::normalize(_characterTransform.getPosition() - cameraPosition);
        cameraTransform.setRotation(glm::quatLookAt(lookingDirection, Y));

    }
} // fmwk