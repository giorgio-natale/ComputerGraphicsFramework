//
// Created by drgio on 24/07/2023.
//

#include "CubeSpawner.h"
#include "GameEngine.h"
#include "MeshComponent.h"
#include "TextureComponent.h"
#include "DefaultMaterial.h"
#include "ColorBlendComponent.h"
#include "SimplePhongMaterial.h"
#include "GGXMaterial.h"

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

            auto sphereModelComponent = std::make_unique<fmwk::MeshComponent>("Mesh", gameEngine->getModelByName("mySphere"));
            auto sphereTextureComponent = std::make_unique<fmwk::TextureComponent>("Texture", gameEngine->getBoundTextureByName("sphereTexture"));
            auto sphereMaterialComponent = std::make_unique<fmwk::GGXMaterial>("Material",  gameEngine->getBoundTextureByName("sphereNormal").getTexture(), gameEngine->getBoundTextureByName("sphereMaterial").getTexture());
            sphereEntity->addComponent(std::move(sphereModelComponent));
            sphereEntity->addComponent(std::move(sphereTextureComponent));
            sphereEntity->addComponent(std::move(sphereMaterialComponent));
            gameEngine->enqueueEntity(std::move(sphereEntity));
            id++;
        }
    }
} // fmwk