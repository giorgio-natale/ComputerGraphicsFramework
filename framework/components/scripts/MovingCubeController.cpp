#include "MovingCubeController.h"
#include "../../Entity.h"
#include "../../GameEngine.h"

namespace fmwk {

    void MovingCubeController::update() {
        auto gameEngine = GameEngine::getInstance();

        Transform& transform = _parentEntity->getTransform();

        bool needsToMove = glm::length(_targetPosition - transform.getPosition()) >= 0.01f;

        if(needsToMove) {
            glm::vec3 targetDirection = glm::normalize(_targetPosition - transform.getPosition());
            float deltaT = gameEngine->getInput().deltaTime;
            glm::vec3 deltaPosition = deltaT * _speed * targetDirection;
            transform.translate(deltaPosition);
        } else {
            transform.setPosition(_targetPosition);
        }

    }

    MovingCubeController::MovingCubeController(const std::string &name, glm::vec3 targetPosition, float speed) : Component(name), _targetPosition(targetPosition), _speed(speed) {}
} // fmwk