//
// Created by Matilde on 22/08/2023.
//

#include <iostream>
#include "PowerUpAddLife.h"
#include "../../GameEngine.h"

namespace fmwk {
    void PowerUpAddLife::applyPowerUp() {
        auto gameEngine = fmwk::GameEngine::getInstance();
        auto&  health = gameEngine->getEntityByName("Character").getHealth();
        health.increaseLife(10);
    }
} // fmwk