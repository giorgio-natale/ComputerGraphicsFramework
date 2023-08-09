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
        // print target positions
        for (int i = 0; i < _targetPositions.size(); ++i) {
            std::cout << "Target position " << i << ": " << _targetPositions[i].x << " " << _targetPositions[i].y << " " << _targetPositions[i].z << std::endl;
        }
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
        for (int i = 0; i < targetPositions.size(); i++) {
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
                            newPositions.emplace_back(targetPositions[i].x - laneWidth / 2 + meshRadius, targetPositions[i].y,
                                                      targetPositions[i].z + laneWidth / 2 + laneWidth * j);
                        } else {
                            newPositions.emplace_back(targetPositions[i].x + laneWidth / 2 - meshRadius, targetPositions[i].y,
                                                      targetPositions[i].z + laneWidth / 2 + laneWidth * j);
                        }
                    }
                } else {
                    newPositions.emplace_back(targetPositions[i].x - laneWidth / 2 + meshRadius, targetPositions[i].y,
                                              targetPositions[i].z - laneWidth / 2);
                    for (int j = 1; j < std::abs(cell_num); j++) {
                        if (j % 2 != 0) {
                            newPositions.emplace_back(targetPositions[i].x + laneWidth / 2 - meshRadius, targetPositions[i].y,
                                                      targetPositions[i].z - laneWidth / 2 - laneWidth * j);
                        } else {
                            newPositions.emplace_back(targetPositions[i].x - laneWidth / 2 + meshRadius, targetPositions[i].y,
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
                            newPositions.emplace_back(targetPositions[i].x + laneWidth / 2 + laneWidth * j, targetPositions[i].y,
                                                      targetPositions[i].z + laneWidth / 2 - meshRadius);
                        } else {
                            newPositions.emplace_back(targetPositions[i].x + laneWidth / 2 + laneWidth * j, targetPositions[i].y,
                                                      targetPositions[i].z - laneWidth / 2 + meshRadius);
                        }
                    }
                } else {
                    newPositions.emplace_back(targetPositions[i].x - laneWidth / 2, targetPositions[i].y,
                                              targetPositions[i].z + laneWidth / 2 - meshRadius);
                    for (int j = 1; j < std::abs(cell_num); j++) {
                        if (j % 2 != 0) {
                            newPositions.emplace_back(targetPositions[i].x - laneWidth / 2 - laneWidth * j, targetPositions[i].y,
                                                      targetPositions[i].z - laneWidth / 2 + meshRadius);
                        } else {
                            newPositions.emplace_back(targetPositions[i].x - laneWidth / 2 - laneWidth * j, targetPositions[i].y,
                                                      targetPositions[i].z + laneWidth / 2 - meshRadius);
                        }
                    }
                }
            }
        }

        targetPositions = newPositions;
    }
} // fmwk

//----------- Target position 0: 0 0 -18
//Target position 1: 2 0 -15
//Target position 2: -2 0 -9
//Target position 3: 2 0 -3
//----------- Target position 4: 0 0 -6
//Target position 5: 2 0 -3
//Target position 6: -2 0 3
//----------- Target position 7: 0 0 0
//Target position 8: 2 0 3
//Target position 9: -2 0 9
//Target position 10: 2 0 15
//Target position 11: -2 0 21
//----------- Target position 12: 0 0 18


//----------- Target position 0: 0 0 -18
//Target position 1: 2 0 -15
//Target position 2: -5 0 -9
//Target position 3: 5 0 -3
//----------- Target position 4: 0 0 -6
//Target position 5: 2 0 -3
//Target position 6: -5 0 3
//----------- Target position 7: 0 0 0
//Target position 8: 2 0 3
//Target position 9: -5 0 9
//Target position 10: 5 0 15
//Target position 11: -5 0 21
//----------- Target position 12: 0 0 18
