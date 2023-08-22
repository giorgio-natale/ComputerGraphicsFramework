//
// Created by Matilde on 22/08/2023.
//

#include <iostream>
#include "PowerUpAddLife.h"
#include "../../GameEngine.h"

namespace fmwk {
    PowerUpAddLife::PowerUpAddLife() {};

    void PowerUpAddLife::applyPowerUp() {
        std::cout << "POWER UP ADD LIFE" << std::endl;
        auto gameEngine = fmwk::GameEngine::getInstance();
        auto&  health = gameEngine->getEntityByName("Character").getHealth();
        health.increaseLife(10);
    }
} // fmwk