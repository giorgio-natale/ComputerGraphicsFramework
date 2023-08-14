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
            // getMazeCollisions si aspetta sempre un vettore normalizzato e un semiasse
            auto info1 = _mazeRepresentation.getMazeCollisions(characterTransform.getPosition(), X);
            std::cout << "[->] Is near: " << std::boolalpha << info1.isNear << " distance: " << info1.distance << std::endl;
            auto info2 = _mazeRepresentation.getMazeCollisions(characterTransform.getPosition(), -X);
            std::cout << "[<-] Is near: " << std::boolalpha << info2.isNear << " distance: " << info2.distance << std::endl;
            auto info3 = _mazeRepresentation.getMazeCollisions(characterTransform.getPosition(), -Z);
            std::cout << "[^] Is near: " << std::boolalpha << info3.isNear << " distance: " << info3.distance << std::endl;
            auto info4 = _mazeRepresentation.getMazeCollisions(characterTransform.getPosition(), Z);
            std::cout << "[v] Is near: " << std::boolalpha << info4.isNear << " distance: " << info4.distance << std::endl;
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
        if(localDirection != glm::vec3(0))
            characterTransform.setRotation(glm::quatLookAt(localDirection, Y));
    }

    // issues
    // se vado contro il muro rallenta la velocità perché una delle componenti è azzerata. Può aver senso compensare sull'altra
    // se ho il centro del cubo oltre lo spigolo di un'intersezione, ma ho ancora la metà posteriore del cubo entro il corridoio,
    // il getMazeCollisions mi dice che non ho nessun muro vicino, poiché calcola le distanze dal centro del cubo.
    // Si potrebbe risolvere calcolando le distanze da due punti invece che uno solo


} // fmwk