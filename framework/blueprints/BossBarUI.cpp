//
// Created by drgio on 22/08/2023.
//

#include "BossBarUI.h"
#include "../GameEngine.h"
#include "../components/scripts/BossBarController.h"
#include "../components/mesh/MeshComponent.h"
#include "../components/texture/TextureComponent.h"
#include "../components/materials/DefaultSprite.h"

namespace fmwk {
    BossBarUI::BossBarUI(Health &bossHealth) : _bossHealth(bossHealth) {}

    void BossBarUI::buildEntity() {
        auto gameEngine = GameEngine::getInstance();
        auto bossBar = std::make_unique<fmwk::Entity>("bossBar", glm::vec2(1, 1), std::vector<fmwk::UiAnchor>{
            {fmwk::BOTTOM, 1.0f}, {fmwk::LEFT, 5.0f}, {fmwk::RIGHT, 5.0f}}, true);
        bossBar->setPreferredRenderOrder(1000);
        bossBar->addComponent(std::make_unique<BossBarController>(_bossHealth));
        bossBar->addComponent(std::make_unique<fmwk::MeshComponent>(gameEngine->getModelByName("spriteModel")));
        bossBar->addComponent(std::make_unique<fmwk::TextureComponent>(gameEngine->getBoundTextureByName("bossBar5")));
        bossBar->addComponent(std::make_unique<fmwk::DefaultSprite>(1.0f));
        gameEngine->enqueueEntity(std::move(bossBar));
    }

} // fmwk