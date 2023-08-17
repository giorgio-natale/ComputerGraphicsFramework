//
// Created by drgio on 11/08/2023.
//

#include "MazeCharacterController.h"
#include "../../systems/InputSystem.h"
#include "../../GameEngine.h"

namespace fmwk {
    MazeCharacterController::MazeCharacterController(const Transform &cameraTransform, const MazeRepresentation &mazeRepresentation, float maxSpeed): Component("MazeCharacterController"),
        _cameraTransform(cameraTransform), _mazeRepresentation(mazeRepresentation), _maxSpeed(maxSpeed), _spacePressConsidered(true)
    {}

    void MazeCharacterController::update() {
        InputResult input = fmwk::GameEngine::getInstance()->getInput();

        auto& characterTransform = _parentEntity->getTransform();

        auto localXGround = glm::normalize(glm::vec3(_cameraTransform.getLocalDirections()[0][0], 0, _cameraTransform.getLocalDirections()[0][2]));
        auto localZGround = glm::normalize(glm::vec3(_cameraTransform.getLocalDirections()[2][0], 0, _cameraTransform.getLocalDirections()[2][2]));
        auto localDirection = (input.m.x == 0 && input.m.z == 0)? glm::vec3(0) : glm::normalize(input.m.x * localXGround + input.m.z * localZGround);

        if(!input.spacePressed)
            _spacePressConsidered = true;
        if(input.spacePressed && _spacePressConsidered){
            auto info1 = _mazeRepresentation.getMazeCollisions(characterTransform.getPosition(), X);
            std::cout << "[->] Is near: " << std::boolalpha << info1.isNear << " distance: " << info1.distance << std::endl;
            auto info2 = _mazeRepresentation.getMazeCollisions(characterTransform.getPosition(), -X);
            std::cout << "[<-] Is near: " << std::boolalpha << info2.isNear << " distance: " << info2.distance << std::endl;
            auto info3 = _mazeRepresentation.getMazeCollisions(characterTransform.getPosition(), -Z);
            std::cout << "[^] Is near: " << std::boolalpha << info3.isNear << " distance: " << info3.distance << std::endl;
            auto info4 = _mazeRepresentation.getMazeCollisions(characterTransform.getPosition(), Z);
            std::cout << "[v] Is near: " << std::boolalpha << info4.isNear << " distance: " << info4.distance << std::endl;

            auto nearSquares = _mazeRepresentation.getBlocksAroundPoint(characterTransform.getPosition());
            _spacePressConsidered = false;
        }

        glm::vec3 constrainedDirection = localDirection;

        if(localDirection != glm::vec3(0)){
            if(localDirection.x > 0) {
                auto info = _mazeRepresentation.getMazeCollisions(characterTransform.getPosition(), X);
                if(info.isNear && info.distance < 0.5) {
                    constrainedDirection.x = 0;
                }
            } else {
                auto info = _mazeRepresentation.getMazeCollisions(characterTransform.getPosition(), -X);
                if(info.isNear && info.distance < 0.5) {
                    constrainedDirection.x = 0;
                }
            }
            if (localDirection.z > 0) {
                auto info = _mazeRepresentation.getMazeCollisions(characterTransform.getPosition(), Z);
                if(info.isNear && info.distance < 0.5) {
                    constrainedDirection.z = 0;
                }
            } else {
                auto info = _mazeRepresentation.getMazeCollisions(characterTransform.getPosition(), -Z);
                if(info.isNear && info.distance < 0.5) {
                    constrainedDirection.z = 0;
                }
            }
        }

        characterTransform.translate(input.deltaTime * _maxSpeed * constrainedDirection);
        auto blocksAround = _mazeRepresentation.getBlocksAroundPoint(characterTransform.getPosition());
        for (auto &square: blocksAround) {
            characterTransform.setPosition(getDistanceFromCharacter(square, characterTransform.getPosition()));
        }
        if(localDirection != glm::vec3(0)){
            characterTransform.setRotation(glm::quatLookAt(localDirection, Y));}
    }

    glm::vec3 MazeCharacterController::getDistanceFromCharacter(Square &square, glm::vec3 position) {
        float distance = 0;
        if(position.x > square.bottomLeftCorner.x && position.x > square.bottomLeftCorner.x + square.edgeSize
            && position.z < square.bottomLeftCorner.z && position.z > square.bottomLeftCorner.z - square.edgeSize) {
            distance = position.x - square.bottomLeftCorner.x - square.edgeSize;
            if (distance < 0.5)
                position.x += 0.5 - distance;
        } else if(position.x < square.bottomLeftCorner.x
                  && position.z < square.bottomLeftCorner.z && position.z > square.bottomLeftCorner.z - square.edgeSize) {
            distance = square.bottomLeftCorner.x - position.x;
            if (distance < 0.5)
                position.x -= 0.5 - distance;
        } else if (position.z < square.bottomLeftCorner.z
                    && position.x > square.bottomLeftCorner.x && position.x < square.bottomLeftCorner.x + square.edgeSize) {
            distance = (square.bottomLeftCorner.z - square.edgeSize) - position.z;
            if (distance < 0.5)
                position.z -= 0.5 - distance;
        } else if (position.z > square.bottomLeftCorner.z
                   && position.x > square.bottomLeftCorner.x && position.x < square.bottomLeftCorner.x + square.edgeSize) {
            distance = position.z - square.bottomLeftCorner.z;
            if (distance < 0.5)
                position.z += 0.5 - distance;
        }
        return position;
    }

} // fmwk