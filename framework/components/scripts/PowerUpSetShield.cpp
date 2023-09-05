//
// Created by Matilde on 22/08/2023.
//

#include <iostream>
#include "PowerUpSetShield.h"
#include "../../GameEngine.h"
#include "../../blueprints/Shield.h"

namespace fmwk {

    void PowerUpSetShield::applyPowerUp() {
        auto gameEngine = fmwk::GameEngine::getInstance();
        if (!gameEngine->doesEntityExist("Shield")) {
            auto &health = gameEngine->getEntityByName("Character").getHealth();
            health.setShield(true);
            Shield().spawnInstance();
        }
    }
} // fmwk