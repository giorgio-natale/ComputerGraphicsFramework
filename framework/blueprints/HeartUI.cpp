//
// Created by drgio on 20/08/2023.
//

#include "HeartUI.h"
#include "../components/materials/DefaultSprite.h"
#include "../components/texture/TextureComponent.h"
#include "../components/mesh/MeshComponent.h"
#include "../GameEngine.h"
#include "../components/scripts/HeartController.h"

namespace fmwk {
    void HeartUI::buildEntity() {
        auto gameEngine = GameEngine::getInstance();
        auto heart = std::make_unique<fmwk::Entity>("heart" + std::to_string(_heartIndex), glm::vec2(1, 1), std::vector<fmwk::UiAnchor>{{fmwk::TOP, 0.5f}, {fmwk::LEFT, (float)_heartIndex + 0.5f}}, false);
        heart->setPreferredRenderOrder(1000);
        heart->addComponent(std::make_unique<HeartController>(_heartIndex));
        heart->addComponent(std::make_unique<fmwk::MeshComponent>(gameEngine->getModelByName("spriteModel")));
        heart->addComponent(std::make_unique<fmwk::TextureComponent>(gameEngine->getBoundTextureByName("heart")));
        heart->addComponent(std::make_unique<fmwk::DefaultSprite>(1.0f));
        gameEngine->enqueueEntity(std::move(heart));
    }

    HeartUI::HeartUI(int heartIndex) : _heartIndex(heartIndex) {}
} // fmwk