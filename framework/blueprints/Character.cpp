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
#include "../components/scripts/CharacterBulletSpawner.h"

namespace fmwk {
    Character::Character(glm::vec3 position) : _position(position){
    }

    void Character::buildEntity() {
        auto gameEngine = GameEngine::getInstance();
        auto cameraEntity = std::make_unique<fmwk::Entity>("Camera", glm::vec3(0, 0, 10), glm::quat(1, 0, 0, 0));
        auto cameraComponent = std::make_unique<fmwk::PerspectiveCamera>(0.1f, 50.0f, glm::radians(45.0f));
        cameraEntity->addComponent(std::move(cameraComponent));

        auto characterEntity = std::make_unique<fmwk::Entity>("Character", _position, glm::quat(1,0,0,0));
        characterEntity->getTransform().setScale({0.5, 0.5, 0.5});

        characterEntity->setPreferredRenderOrder(0);

        characterEntity->addComponent(std::make_unique<fmwk::MeshComponent>(gameEngine->getModelByName("myCube")));
        characterEntity->addComponent(std::make_unique<fmwk::Collider>(1.0f, "CHARACTER", glm::vec3(0,0,0)));
        characterEntity->addComponent(std::make_unique<fmwk::TextureComponent>(gameEngine->getBoundTextureByName("cubeTexture")));
        characterEntity->addComponent(std::make_unique<fmwk::SimplePhongMaterial>());
        characterEntity->addComponent(std::make_unique<fmwk::Health>(30.0f,1.0f));
        std::unordered_set<std::string> characterBulletTags = std::unordered_set<std::string>{"ENEMY"};
        characterEntity->addComponent(std::make_unique<CharacterBulletSpawner>(glm::vec3(0,0,0), 8.5f, 1.0f, &characterBulletTags));
        characterEntity->addComponent(std::make_unique<fmwk::CharacterController>("CharacterController", cameraEntity->getTransform(), 5.0f));
        cameraEntity->addComponent(std::make_unique<fmwk::CameraController>("CameraController", characterEntity->getTransform(), glm::radians(120.0f), 3.0f, 0.25f));

        gameEngine->enqueueEntity(std::move(cameraEntity));
        gameEngine->enqueueEntity(std::move(characterEntity));


    }
} // fmwk