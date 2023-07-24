//
// Created by drgio on 20/07/2023.
//

#include "DefaultMaterial.h"
#include "GameEngine.h"
#include "MeshComponent.h"
#include "TextureComponent.h"

namespace fmwk {
    void DefaultMaterial::updateDescriptorSet(int currentImage) {
        DefaultMaterialUniformBlock ubo{};
        ubo.brightness = _brightness;
        _descriptorSet->map(currentImage, &ubo, sizeof(ubo), 0);
    }

    void DefaultMaterial::update() {
        static int id = 0;
        auto gameEngine = GameEngine::getInstance();
        _brightness -= 0.01f;
        if(_brightness <= 0) {

            /*id++;
            auto cubeEntity = std::make_unique<fmwk::Entity>("myCubeEntity" + std::to_string(id));
            auto modelComponent = std::make_unique<fmwk::MeshComponent>("Mesh", gameEngine->getModelByName("myCube"));
            auto textureComponent = std::make_unique<fmwk::TextureComponent>("Texture", gameEngine->getBoundTextureByName("cubeTexture"));
            auto materialComponent = std::make_unique<fmwk::DefaultMaterial>("Material", 1.5f);
            cubeEntity->addComponent(std::move(modelComponent));
            cubeEntity->addComponent(std::move(textureComponent));
            cubeEntity->addComponent(std::move(materialComponent));

            gameEngine->enqueueEntityRemoval(_parentEntity->getName());
            gameEngine->enqueueEntity(std::move(cubeEntity));
             */
            auto materialComponent = std::make_unique<fmwk::DefaultMaterial>("Material", 1.5f);
            _parentEntity->enqueueComponentRemoval(_name);
            _parentEntity->enqueueComponent(std::move(materialComponent));

        }
    }

    DescriptorSetClaim DefaultMaterial::getDescriptorSetClaim() {
        return {{0, UNIFORM, sizeof(DefaultMaterialUniformBlock)}};
    }

    DefaultMaterial::DefaultMaterial(const std::string &name,float brightness) : MaterialComponent(name, DEFAULT), _brightness(brightness){}
} // fmwk