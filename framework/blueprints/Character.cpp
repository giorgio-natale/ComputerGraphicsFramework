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
#include "../components/scripts/CameraController.h"
#include "../GameEngine.h"
#include "../components/scripts/CharacterBulletSpawner.h"
#include "../components/materials/SimplePhongBlinkMaterial.h"
#include "../components/scripts/MazeCharacterController.h"
#include "HeartUI.h"
#include "../components/scripts/CharacterDespawner.h"

namespace fmwk {
    Character::Character(glm::vec3 position, MazeRepresentation &mazeRepresentation) : _position(position), _mazeRepresentation(mazeRepresentation){
    }

    void Character::buildEntity() {
        auto gameEngine = GameEngine::getInstance();
        auto cameraEntity = std::make_unique<fmwk::Entity>("Camera", glm::vec3(0, 0, 10), glm::quat(1, 0, 0, 0));
        auto cameraComponent = std::make_unique<fmwk::PerspectiveCamera>(0.001f, 60.0f, glm::radians(45.0f));
        cameraEntity->addComponent(std::move(cameraComponent));

        auto characterEntity = std::make_unique<fmwk::Entity>("Character", _position, glm::quat(1,0,0,0));

        characterEntity->setPreferredRenderOrder(0);

        characterEntity->addComponent(std::make_unique<fmwk::MeshComponent>(gameEngine->getModelByName("smoothPumpkin")));
        characterEntity->addComponent(std::make_unique<fmwk::Collider>(0.5f, "CHARACTER", glm::vec3(0,0,0)));
        characterEntity->addComponent(std::make_unique<fmwk::TextureComponent>(gameEngine->getBoundTextureByName("dungeonTexture")));
        characterEntity->addComponent(std::make_unique<SimplePhongBlinkMaterial>(4.0f, 0.5f));
        characterEntity->addComponent(std::make_unique<fmwk::Health>(30.0f,3.0f, 60.0f));
        std::unordered_set<std::string> characterBulletTags = std::unordered_set<std::string>{"ENEMY"};
        characterEntity->addComponent(std::make_unique<CharacterBulletSpawner>(glm::vec3(0,0,0), 12.0f, 1.0f, &characterBulletTags));
        characterEntity->addComponent(std::make_unique<fmwk::MazeCharacterController>(cameraEntity->getTransform(), _mazeRepresentation, 5.0f));
        characterEntity->addComponent(std::make_unique<fmwk::CharacterDespawner>());

        cameraEntity->addComponent(std::make_unique<fmwk::CameraController>("CameraController", characterEntity->getTransform(), glm::radians(120.0f), 4.0f, 1.0f));

        HeartUI(0).addInstance();
        HeartUI(1).addInstance();
        HeartUI(2).addInstance();
        HeartUI(3).addInstance();
        HeartUI(4).addInstance();
        HeartUI(5).addInstance();

        gameEngine->enqueueEntity(std::move(cameraEntity));
        gameEngine->enqueueEntity(std::move(characterEntity));


    }
} // fmwk