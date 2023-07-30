//
// Created by drgio on 04/05/2023.
//

#include <memory>
#include <iostream>
#include "CameraController.h"
#include "../../GameEngine.h"
#include "../../Entity.h"
#include "../../utils.h"

namespace fmwk {
    CameraController::CameraController(const std::string &name, const Transform &characterTransform,
                                       float angularSpeed, float cameraDistance, float cameraHeight) : Component(name),
                                                                   _characterTransform(characterTransform),
                                                                   _angularSpeed(angularSpeed),
                                                                   _cameraDistance(cameraDistance),
                                                                   _cameraHeight(cameraHeight){}

    void CameraController::postUpdate() {
        InputResult input = fmwk::GameEngine::getInstance()->getInput();

        auto& cameraTransform =_parentEntity->getTransform();;
        _rotationAboutCenter = _rotationAboutCenter + glm::vec3(input.r.x * _angularSpeed * input.deltaTime,input.r.y * _angularSpeed * input.deltaTime,0);
        _rotationAboutCenter = {glm::clamp(_rotationAboutCenter[0], glm::radians(-8.75f), glm::radians(60.0f)), _rotationAboutCenter[1] , _rotationAboutCenter[2]};

        auto cameraBasePosition = _characterTransform.getPosition() + glm::vec3(0, _cameraHeight, _cameraDistance);
        auto cameraPosition = glm::vec3 (rotationAroundPoint(
                _characterTransform.getPosition(),
                _rotationAboutCenter[1],
                _rotationAboutCenter[0]) * glm::vec4(cameraBasePosition, 1));

        cameraTransform.setPosition(cameraPosition);
        auto lookingDirection = glm::normalize(_characterTransform.getPosition() - cameraPosition);
        cameraTransform.setRotation(glm::quatLookAt(lookingDirection, Y));

    }
} // fmwk