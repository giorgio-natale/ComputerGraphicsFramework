//
// Created by drgio on 24/07/2023.
//

#include "CubeSpawner.h"
#include "GameEngine.h"
#include "MeshComponent.h"
#include "TextureComponent.h"
#include "DefaultMaterial.h"
#include "ColorBlendComponent.h"

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

            auto cubeEntity = std::make_unique<fmwk::Entity>("spawnedCube" + std::to_string(id),
                                                             transform.getPosition(), transform.getRotation());
            auto modelComponent = std::make_unique<fmwk::MeshComponent>("Mesh", gameEngine->getModelByName("myCube"));
            auto textureComponent = std::make_unique<fmwk::TextureComponent>("Texture",
                                                                             gameEngine->getBoundTextureByName(
                                                                                     "cubeTexture"));
            auto materialComponent = std::make_unique<fmwk::ColorBlendComponent>("Material", glm::vec3(255, 0, 0), 0.3f);
            cubeEntity->addComponent(std::move(modelComponent));
            cubeEntity->addComponent(std::move(textureComponent));
            cubeEntity->addComponent(std::move(materialComponent));

            gameEngine->enqueueEntity(std::move(cubeEntity));
            id++;
        }
    }
} // fmwk