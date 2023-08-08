#include "EnemyController.h"
#include "../../Entity.h"
#include "../../GameEngine.h"

namespace fmwk {
    EnemyController::EnemyController(const std::vector<glm::vec3> &targetPositions,
                                     float speed, float laneWidth, float meshRadius) : Component("EnemyController"),
                                                                                       _speed(speed),
                                                                                       _laneWidth(laneWidth),
                                                                                       _meshRadius(meshRadius) {
        processVector(const_cast<std::vector<glm::vec3> &>(targetPositions));
        _targetPositions = targetPositions;
    }

    void EnemyController::update() {
        auto gameEngine = GameEngine::getInstance();

        Transform& transform = _parentEntity->getTransform();

        bool needsToMove = glm::length(_targetPositions[_currentTargetIndex] - transform.getPosition()) >= 0.01f;

        if(needsToMove) {
            glm::vec3 targetDirection = glm::normalize(_targetPositions[_currentTargetIndex] - transform.getPosition());
            float deltaT = gameEngine->getInput().deltaTime;
            glm::vec3 deltaPosition = deltaT * _speed * targetDirection;
            transform.translate(deltaPosition);
        } else {
            transform.setPosition(_targetPositions[_currentTargetIndex]);
            _currentTargetIndex = (_currentTargetIndex + 1) % _targetPositions.size();
        }
    }

    void EnemyController::processVector(std::vector<glm::vec3> &targetPositions) {
        for (int i = targetPositions.size() - 2; i >= 0; --i)
            targetPositions.emplace_back(targetPositions[i]);

    }
} // fmwk