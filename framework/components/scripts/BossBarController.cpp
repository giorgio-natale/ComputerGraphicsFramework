//
// Created by drgio on 22/08/2023.
//

#include "BossBarController.h"
#include "../../Entity.h"
#include "../texture/TextureComponent.h"
#include "../../GameEngine.h"
#include "Despawner.h"

namespace fmwk {
    BossBarController::BossBarController(Health &bossHealth) : Component("BossHealthController"),
                                                               _bossHealth(bossHealth) {}

    void BossBarController::postUpdate() {
        auto gameEngine = GameEngine::getInstance();

        if(_bossHealth.getCurrentLifeQuantity() > 0) {
            int healthLevel = (int) (_bossHealth.getCurrentLifeQuantity() / 10.0f);
            reinterpret_cast<TextureComponent &>(_parentEntity->getComponentByName("Texture")).setBoundTexture(
                    gameEngine->getBoundTextureByName("bossBar" + std::to_string(healthLevel)));
        }else{
            _parentEntity->markForRemoval();
        }
    }
} // fmwk