//
// Created by drgio on 23/08/2023.
//

#include "SplashScreen.h"
#include "../GameEngine.h"
#include "../components/texture/TextureComponent.h"
#include "../components/mesh/MeshComponent.h"
#include "../components/materials/DefaultSprite.h"

#include <utility>

namespace fmwk {
    SplashScreen::SplashScreen(std::string textureName) : _textureName(std::move(textureName)) {}

    void SplashScreen::buildEntity() {
        auto gameEngine = GameEngine::getInstance();
        auto splash = std::make_unique<fmwk::Entity>("splash", glm::vec2(1, 3), std::vector<fmwk::UiAnchor>{
                {fmwk::TOP, 0}, {fmwk::LEFT, 0}, {fmwk::RIGHT, 0}}, false);
        splash->setPreferredRenderOrder(1000);
        splash->addComponent(std::make_unique<fmwk::MeshComponent>(gameEngine->getModelByName("spriteModel")));
        splash->addComponent(std::make_unique<fmwk::TextureComponent>(gameEngine->getBoundTextureByName(_textureName)));
        splash->addComponent(std::make_unique<fmwk::DefaultSprite>(1.0f));
        gameEngine->enqueueEntity(std::move(splash));
    }
} // fmwk