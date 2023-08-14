//
// Created by drgio on 24/07/2023.
//

#include "CubeSpawner.h"
#include "../../GameEngine.h"
#include "../mesh/MeshComponent.h"
#include "../texture/TextureComponent.h"
#include "../materials/DefaultMaterial.h"
#include "../materials/ColorBlendComponent.h"
#include "../materials/SimplePhongMaterial.h"
#include "../materials/GGXMaterial.h"
#include "EntityDespawner.h"

namespace fmwk {
    CubeSpawner::CubeSpawner(const std::string &name) : Component(name) {}

    void CubeSpawner::update() {
        static int id = 0;
        auto gameEngine = GameEngine::getInstance();
        if(!gameEngine->getInput().spacePressed)
            _spawnEnabled = true;
        if(gameEngine->getInput().spacePressed && _spawnEnabled) {
            _spawnEnabled = false;
            auto &transform = _parentEntity->getTransform();

            auto sphereEntity = std::make_unique<fmwk::Entity>("spawnedSphere" + std::to_string(id), transform.getPosition(), transform.getRotation());
            //sphereEntity->getTransform().setScale(glm::vec3(0.5f, 0.5f, 0.5f));
            sphereEntity->addComponent(std::make_unique<fmwk::Collider>(1.0f, "BULLET", glm::vec3(0,0,0)));
            sphereEntity->addComponent(std::make_unique<fmwk::MeshComponent>(gameEngine->getModelByName("mySphere")));
            sphereEntity->addComponent(std::make_unique<fmwk::TextureComponent>(gameEngine->getBoundTextureByName("sphereTexture")));
            sphereEntity->addComponent(std::make_unique<fmwk::GGXMaterial>(gameEngine->getBoundTextureByName("sphereNormal").getTexture(), gameEngine->getBoundTextureByName("sphereMaterial").getTexture()));
            sphereEntity->addComponent(std::make_unique<fmwk::EntityDespawner>("Despawner", 6.0f));

            gameEngine->enqueueEntity(std::move(sphereEntity));
            id++;
        }
    }
} // fmwk