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

namespace fmwk {
    CubeSpawner::CubeSpawner(const std::string &name) : Component(name) {}

    void CubeSpawner::update() {
        static int id = 0;
        auto gameEngine = GameEngine::getInstance();
        if(!gameEngine->getInput().spacePressed)
            _spawnEnabled = true;

        /*if(gameEngine->getInput().spacePressed && _spawnEnabled) {
            _spawnEnabled = false;
            auto &transform = _parentEntity->getTransform();

            auto cubeEntity = std::make_unique<fmwk::Entity>("spawnedCube" + std::to_string(id),
                                                             transform.getPosition(), transform.getRotation());
            auto modelComponent = std::make_unique<fmwk::MeshComponent>("Mesh", gameEngine->getModelByName("myCube"));
            auto textureComponent = std::make_unique<fmwk::TextureComponent>("Texture",
                                                                             gameEngine->getBoundTextureByName(
                                                                                     "cubeTexture"));
            //auto materialComponent = std::make_unique<fmwk::ColorBlendComponent>("Material", glm::vec3(255, 0, 0), 0.3f);
            auto materialComponent = std::make_unique<fmwk::SimplePhongMaterial>("Material");

            cubeEntity->addComponent(std::move(modelComponent));
            cubeEntity->addComponent(std::move(textureComponent));
            cubeEntity->addComponent(std::move(materialComponent));

            gameEngine->enqueueEntity(std::move(cubeEntity));
            id++;
        } */
        if(gameEngine->getInput().spacePressed && _spawnEnabled) {
            _spawnEnabled = false;
            auto &transform = _parentEntity->getTransform();

            auto sphereEntity = std::make_unique<fmwk::Entity>("spawnedSphere" + std::to_string(id), transform.getPosition(), transform.getRotation());

            auto sphereModelComponent = std::make_unique<fmwk::MeshComponent>(gameEngine->getModelByName("mySphere"));
            auto sphereTextureComponent = std::make_unique<fmwk::TextureComponent>(gameEngine->getBoundTextureByName("sphereTexture"));
            auto sphereMaterialComponent = std::make_unique<fmwk::GGXMaterial>(gameEngine->getBoundTextureByName("sphereNormal").getTexture(), gameEngine->getBoundTextureByName("sphereMaterial").getTexture());
            sphereEntity->addComponent(std::move(sphereModelComponent));
            sphereEntity->addComponent(std::move(sphereTextureComponent));
            sphereEntity->addComponent(std::move(sphereMaterialComponent));
            gameEngine->enqueueEntity(std::move(sphereEntity));
            id++;
        }
    }
} // fmwk