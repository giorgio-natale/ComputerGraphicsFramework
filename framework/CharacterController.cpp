//
// Created by drgio on 03/05/2023.
//

#include <memory>
#include <iostream>
#include "CharacterController.h"
#include "GameEngine.h"
#include "Entity.h"

namespace fmwk {
    CharacterController::CharacterController(const std::string &name, const Transform &cameraTransform, float maxSpeed) : Component(
            name), _cameraTransform(cameraTransform), _maxSpeed(maxSpeed){}

    void CharacterController::update() {
        InputResult input = fmwk::GameEngine::getInstance()->getInput();

        auto& characterTransform = _parentEntity->getTransform();


        auto localXGround = glm::normalize(glm::vec3(_cameraTransform.getLocalDirections()[0][0], 0, _cameraTransform.getLocalDirections()[0][2]));
        auto localZGround = glm::normalize(glm::vec3(_cameraTransform.getLocalDirections()[2][0], 0, _cameraTransform.getLocalDirections()[2][2]));
        auto localDirection = (input.m.x == 0 && input.m.z == 0)? glm::vec3(0) : glm::normalize(input.m.x * localXGround + input.m.z * localZGround);
        characterTransform.translate(input.deltaTime * _maxSpeed * localDirection);
        if(localDirection != glm::vec3(0))
            characterTransform.setRotation(glm::quatLookAt(localDirection, Y));
    }

} // fmwk