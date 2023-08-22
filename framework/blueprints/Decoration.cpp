//
// Created by drgio on 22/08/2023.
//

#include "Decoration.h"
#include "../components/mesh/MeshComponent.h"
#include "../GameEngine.h"
#include "../components/materials/SimplePhongMaterial.h"
#include "../components/texture/TextureComponent.h"

#include <utility>

namespace fmwk {
    Decoration::Decoration(std::string name, const glm::vec3 &position, const glm::quat &rotation, const glm::vec3 &scale,
                           std::string textureName) : _name(std::move(name)), _position(position), _rotation(rotation), _scale(scale),
                                                      _modelName(std::move(textureName)) {}

    Decoration::Decoration(const glm::vec3 &position, const glm::quat &rotation, const glm::vec3 &scale,
                           std::string modelName) : Decoration("decoration" + std::to_string(getNewNumber()), position, rotation, scale, std::move(modelName)){}


    void Decoration::buildEntity() {
        auto gameEngine = GameEngine::getInstance();
        auto decorationEntity = std::make_unique<fmwk::Entity>(_name, _position,_rotation);
        decorationEntity->getTransform().setScale(_scale);
        decorationEntity->addComponent(std::make_unique<fmwk::MeshComponent>(gameEngine->getModelByName(_modelName)));
        decorationEntity->addComponent(std::make_unique<fmwk::TextureComponent>(gameEngine->getBoundTextureByName("dungeonTexture")));

        decorationEntity->addComponent(std::make_unique<SimplePhongMaterial>());
        gameEngine->enqueueEntity(std::move(decorationEntity));

    }
} // fmwk