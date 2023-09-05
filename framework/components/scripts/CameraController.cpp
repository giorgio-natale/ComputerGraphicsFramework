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
                                                                   _cameraHeight(cameraHeight),
                                                                   _mazeRepresentation(nullptr){}

    void CameraController::postUpdate() {
        auto gameEngine = GameEngine::getInstance();
        InputResult input = gameEngine->getInput();
        if(_mazeRepresentation == nullptr)
            _mazeRepresentation = &reinterpret_cast<fmwk::MazeRepresentation&>(gameEngine->getEntityByName("Maze").getComponentByName("MazeRepresentation"));

        auto& cameraTransform =_parentEntity->getTransform();
        _rotationAboutCenter = _rotationAboutCenter + glm::vec3(input.r.x * _angularSpeed * input.deltaTime,input.r.y * _angularSpeed * input.deltaTime,0);
        _rotationAboutCenter = {glm::clamp(_rotationAboutCenter[0], glm::radians(-8.75f), glm::radians(60.0f)), _rotationAboutCenter[1] , _rotationAboutCenter[2]};
        auto cameraTarget = _characterTransform.getPosition() + glm::vec3(0, _cameraHeight, 0);
        auto cameraBasePosition = cameraTarget + glm::vec3(0, 0, _cameraDistance);
        auto cameraPosition = glm::vec3 (rotationAroundPoint(
                _characterTransform.getPosition(),
                _rotationAboutCenter[1],
                _rotationAboutCenter[0]) * glm::vec4(cameraBasePosition, 1));

        cameraTransform.setPosition(cameraPosition);
        if(_mazeRepresentation->isPositionInsideBlock(cameraPosition)){
            /*
            auto blocksAround = _mazeRepresentation->getBlocksAroundPoint(_characterTransform.getPosition());
            std::vector<float> lambdas{};
            for (auto &square: blocksAround) {
                float lambda = getLambda(square, cameraTarget, cameraTransform.getPosition());
                if(lambda >= 0)
                    lambdas.push_back(lambda);
            }

            auto minLambda = std::min_element(lambdas.begin(), lambdas.end());
            if(minLambda != lambdas.end() && *minLambda < 1000) {
                glm::vec3 directionFromCharacterToCamera = glm::normalize(
                        cameraTransform.getPosition() - cameraTarget);
                cameraTransform.setPosition(
                        cameraTarget + directionFromCharacterToCamera * ((*minLambda) - 0.1f));
            }
             */
            cameraTransform.setPosition(getPointClosestToBoxFace(_mazeRepresentation->getBlockOfPoint(cameraTransform.getPosition())[0], cameraTarget, cameraTransform.getPosition()));
        }
        auto lookingDirection = glm::normalize(_characterTransform.getPosition() - cameraTransform.getPosition());
        cameraTransform.setRotation(glm::quatLookAt(lookingDirection, Y));

    }

    float CameraController::getLambda(Square &square, glm::vec3 characterPosition, glm::vec3 cameraPosition) {
        glm::vec3 v = glm::normalize(cameraPosition - characterPosition);
        std::vector<float> lambdas{};
        //SOUTH FACE
        glm::vec3 n = Z;

        square.bottomLeftCorner.y = 0;
        glm::vec3 p = square.bottomLeftCorner;
        float lambda = glm::dot(n, v) == 0 ? 1000 : (glm::dot(n, p) - glm::dot(n, characterPosition)) / glm::dot(n, v);
        if(lambda >= 0)
            lambdas.push_back(lambda);

        //WEST FACE
        n = -X;
        p = square.bottomLeftCorner;
        lambda = glm::dot(n, v) == 0 ? 1000 : (glm::dot(n, p) - glm::dot(n, characterPosition)) / glm::dot(n, v);
        if(lambda >= 0)
            lambdas.push_back(lambda);

        //NORTH FACE
        n = -Z;
        p = square.bottomLeftCorner + glm::vec3(square.edgeSize, 0, -square.edgeSize);
        lambda = glm::dot(n, v) == 0 ? 1000 : (glm::dot(n, p) - glm::dot(n, characterPosition)) / glm::dot(n, v);
        if(lambda >= 0)
            lambdas.push_back(lambda);

        //EAST FACE
        n = X;
        p = square.bottomLeftCorner + glm::vec3(square.edgeSize, 0, -square.edgeSize);
        lambda = glm::dot(n, v) == 0 ? 1000 : (glm::dot(n, p) - glm::dot(n, characterPosition)) / glm::dot(n, v);
        if(lambda >= 0)
            lambdas.push_back(lambda);

        auto minElement = std::min_element(lambdas.begin(), lambdas.end());
        return minElement != lambdas.end() ? *minElement : 10000;
    }

    glm::vec3
    CameraController::getPointClosestToBoxFace(Square &square, glm::vec3 characterPosition, glm::vec3 cameraPosition) {
        square.bottomLeftCorner.y = 0;
        std::vector<std::pair<glm::vec3, glm::vec3>> planes{}; //normal and point

        //point of cube next to character
        float xNearestPoint = glm::clamp(characterPosition.x, square.bottomLeftCorner.x, square.bottomLeftCorner.x + square.edgeSize);
        float yNearestPoint = glm::clamp(characterPosition.y, 0.0f, square.edgeSize);
        float zNearestPoint = glm::clamp(characterPosition.z, square.bottomLeftCorner.z - square.edgeSize, square.bottomLeftCorner.z);
        glm::vec3 pointOfBoxNearestToCharacter = glm::vec3(xNearestPoint, yNearestPoint, zNearestPoint);
        glm::vec3 v = glm::normalize(characterPosition - pointOfBoxNearestToCharacter);

        planes.emplace_back(Z, square.bottomLeftCorner);
        planes.emplace_back(-X, square.bottomLeftCorner);
        planes.emplace_back(-Z, square.bottomLeftCorner + glm::vec3(square.edgeSize, 0, -square.edgeSize));
        planes.emplace_back(X, square.bottomLeftCorner + glm::vec3(square.edgeSize, 0, -square.edgeSize));

        std::vector<float> lambdas{};
        for(auto& [n, p] : planes){
            if(glm::dot(n, v) != 0){
                float lambda = (glm::dot(n, p) - glm::dot(n, cameraPosition)) / glm::dot(n, v);
                if(lambda >= 0)
                    lambdas.push_back(lambda);
            }
        }

        auto minElement = std::min_element(lambdas.begin(), lambdas.end());
        if(minElement == lambdas.end())
            return {0,0,0};

        return cameraPosition + v * (*minElement + 0.2f);
    }
} // fmwk