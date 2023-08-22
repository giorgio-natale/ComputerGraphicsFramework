#include "EnemyController.h"
#include "../../Entity.h"
#include "../../GameEngine.h"

namespace fmwk {
    EnemyController::EnemyController(const std::vector<glm::vec3> &targetPositions,
                                     float speed, float laneWidth, float meshRadius) : Component("EnemyController"),
                                                                                       _speed(speed),
                                                                                       _laneWidth(laneWidth),
                                                                                       _meshRadius(meshRadius) {
        findZPath(const_cast<std::vector<glm::vec3> &>(targetPositions), laneWidth, meshRadius);
        processVector(const_cast<std::vector<glm::vec3> &>(targetPositions));
        _targetPositions = targetPositions;
    }

    void EnemyController::update() {
        auto gameEngine = GameEngine::getInstance();

        Transform& transform = _parentEntity->getTransform();

        bool needsToMove = glm::length(_targetPositions[_currentTargetIndex] - transform.getPosition()) >= 0.1f;

        if(needsToMove) {
            glm::vec3 targetDirection = glm::normalize(_targetPositions[_currentTargetIndex] - transform.getPosition());
            float deltaT = gameEngine->getInput().deltaTime;
            glm::vec3 deltaPosition = deltaT * _speed * targetDirection;
            transform.translate(deltaPosition);
        } else {
            glm::vec3 targetDirection = glm::normalize(_targetPositions[_currentTargetIndex + 1] - transform.getPosition());
            transform.setRotation(glm::quatLookAt(targetDirection, Y));
            transform.setPosition(_targetPositions[_currentTargetIndex]);
            _currentTargetIndex = (_currentTargetIndex + 1) % _targetPositions.size();
        }
    }

    void EnemyController::processVector(std::vector<glm::vec3> &targetPositions) {
        for (int i = targetPositions.size() - 2; i >= 0; --i)
            targetPositions.emplace_back(targetPositions[i]);

    }

    void EnemyController::findZPath(std::vector<glm::vec3> &targetPositions, float laneWidth, float meshRadius) {
        std::vector<glm::vec3> newPositions;
        for (int i = 0; i < targetPositions.size() - 1; i++) {
            int x_pos = 0;
            int z_pos = 0;
            int cell_num = 0;
            std::string cell_dir;
            if (targetPositions[i].x == targetPositions[i + 1].x) {
                cell_num = (int) ((targetPositions[i].z - targetPositions[i + 1].z) / laneWidth);
                cell_dir = "z";
            } else {
                cell_num = (int) ((targetPositions[i].x - targetPositions[i + 1].x) / laneWidth);
                cell_dir = "x";
            }
            newPositions.emplace_back(targetPositions[i]);
            if (cell_dir == "z") {
                if (cell_num < 0) {
                    newPositions.emplace_back(targetPositions[i].x + laneWidth / 2 - meshRadius, targetPositions[i].y,
                                              targetPositions[i].z + laneWidth / 2);
                    for (int j = 1; j < std::abs(cell_num); j++) {
                        if (j % 2 != 0) {
                            newPositions.emplace_back(targetPositions[i].x - laneWidth / 2 + meshRadius,
                                                      targetPositions[i].y,
                                                      targetPositions[i].z + laneWidth / 2 + laneWidth * j);
                        } else {
                            newPositions.emplace_back(targetPositions[i].x + laneWidth / 2 - meshRadius,
                                                      targetPositions[i].y,
                                                      targetPositions[i].z + laneWidth / 2 + laneWidth * j);
                        }
                    }
                } else {
                    newPositions.emplace_back(targetPositions[i].x - laneWidth / 2 + meshRadius, targetPositions[i].y,
                                              targetPositions[i].z - laneWidth / 2);
                    for (int j = 1; j < std::abs(cell_num); j++) {
                        if (j % 2 != 0) {
                            newPositions.emplace_back(targetPositions[i].x + laneWidth / 2 - meshRadius,
                                                      targetPositions[i].y,
                                                      targetPositions[i].z - laneWidth / 2 - laneWidth * j);
                        } else {
                            newPositions.emplace_back(targetPositions[i].x - laneWidth / 2 + meshRadius,
                                                      targetPositions[i].y,
                                                      targetPositions[i].z - laneWidth / 2 - laneWidth * j);
                        }
                    }
                }
            } else {
                if (cell_num < 0) {
                    newPositions.emplace_back(targetPositions[i].x + laneWidth / 2, targetPositions[i].y,
                                              targetPositions[i].z - laneWidth / 2 + meshRadius);
                    for (int j = 1; j < std::abs(cell_num); j++) {
                        if (j % 2 != 0) {
                            newPositions.emplace_back(targetPositions[i].x + laneWidth / 2 + laneWidth * j,
                                                      targetPositions[i].y,
                                                      targetPositions[i].z + laneWidth / 2 - meshRadius);
                        } else {
                            newPositions.emplace_back(targetPositions[i].x + laneWidth / 2 + laneWidth * j,
                                                      targetPositions[i].y,
                                                      targetPositions[i].z - laneWidth / 2 + meshRadius);
                        }
                    }
                } else {
                    newPositions.emplace_back(targetPositions[i].x - laneWidth / 2, targetPositions[i].y,
                                              targetPositions[i].z + laneWidth / 2 - meshRadius);
                    for (int j = 1; j < std::abs(cell_num); j++) {
                        if (j % 2 != 0) {
                            newPositions.emplace_back(targetPositions[i].x - laneWidth / 2 - laneWidth * j,
                                                      targetPositions[i].y,
                                                      targetPositions[i].z - laneWidth / 2 + meshRadius);
                        } else {
                            newPositions.emplace_back(targetPositions[i].x - laneWidth / 2 - laneWidth * j,
                                                      targetPositions[i].y,
                                                      targetPositions[i].z + laneWidth / 2 - meshRadius);
                        }
                    }
                }
            }
        }

        targetPositions = newPositions;

    }
} // fmwk


