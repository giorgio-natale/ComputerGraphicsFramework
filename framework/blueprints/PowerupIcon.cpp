//
// Created by drgio on 28/08/2023.
//

#include "PowerupIcon.h"
#include "../GameEngine.h"
#include "../components/texture/TextureComponent.h"
#include "../components/mesh/MeshComponent.h"
#include "../components/materials/DefaultSprite.h"

namespace fmwk {
    PowerupIcon::PowerupIcon(float topOffset, const std::string &textureName) : _topOffset(topOffset),
                                                                                _textureName(textureName) {}
    void PowerupIcon::buildEntity() {
        auto gameEngine = GameEngine::getInstance();
        auto icon = std::make_unique<fmwk::Entity>("icon" + std::to_string(getNewNumber()), glm::vec2(1, 1), std::vector<fmwk::UiAnchor>{{fmwk::TOP, _topOffset}, {fmwk::RIGHT, 0.5f}}, false);
        icon->setPreferredRenderOrder(1000);
        icon->addComponent(std::make_unique<fmwk::MeshComponent>(gameEngine->getModelByName("spriteModel")));
        icon->addComponent(std::make_unique<fmwk::TextureComponent>(gameEngine->getBoundTextureByName(_textureName)));
        icon->addComponent(std::make_unique<fmwk::DefaultSprite>(1.0f));
        gameEngine->enqueueEntity(std::move(icon));

    }


} // fmwk