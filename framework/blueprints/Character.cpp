//
// Created by drgio on 19/08/2023.
//

#include <memory>
#include "Character.h"
#include "../Entity.h"
#include "../components/camera/PerspectiveCamera.h"
#include "../components/mesh/MeshComponent.h"
#include "../components/texture/TextureComponent.h"
#include "../components/materials/SimplePhongMaterial.h"
#include "../components/scripts/CubeSpawner.h"
#include "../components/scripts/CharacterController.h"
#include "../components/scripts/CameraController.h"
#include "../GameEngine.h"

namespace fmwk {
    Character::Character(glm::vec3 position, glm::quat orientation) : _position(position), _orientation(orientation){
    }

    void Character::buildEntity() {
        auto gameEngine = GameEngine::getInstance();
        auto cameraEntity = std::make_unique<fmwk::Entity>("Camera", glm::vec3(0, 0, 10), glm::quat(1, 0, 0, 0));
        auto cameraComponent = std::make_unique<fmwk::PerspectiveCamera>(0.1f, 100.0f, glm::radians(45.0f));
        cameraEntity->addComponent(std::move(cameraComponent));

        auto cubeEntity = std::make_unique<fmwk::Entity>("Cube", _position, _orientation);
        cubeEntity->addComponent(std::make_unique<fmwk::MeshComponent>(gameEngine->getModelByName("myCube")));
        cubeEntity->addComponent(std::make_unique<fmwk::TextureComponent>(gameEngine->getBoundTextureByName("cubeTexture")));
        cubeEntity->addComponent(std::make_unique<fmwk::SimplePhongMaterial>());
        cubeEntity->addComponent(std::make_unique<fmwk::CubeSpawner>("CubeSpawner"));

        cubeEntity->addComponent(std::make_unique<fmwk::CharacterController>("CharacterController", cameraEntity->getTransform(), 4.0f));
        cameraEntity->addComponent(std::make_unique<fmwk::CameraController>("CameraController", cubeEntity->getTransform(), glm::radians(120.0f), 8.0f, 0.25f));

        gameEngine->enqueueEntity(std::move(cameraEntity));
        gameEngine->enqueueEntity(std::move(cubeEntity));

    }
} // fmwk