//
// Created by Matilde on 22/08/2023.
//

#include <iostream>
#include "PowerUpIncreaseSpeed.h"
#include "../../GameEngine.h"
#include "MazeCharacterController.h"
#include "../../blueprints/PowerupIcon.h"

namespace fmwk {
    void PowerUpIncreaseSpeed::applyPowerUp() {
        auto gameEngine = fmwk::GameEngine::getInstance();
        auto&  characterController = reinterpret_cast<fmwk::MazeCharacterController&>(gameEngine->getEntityByName("Character").getComponentByName("MazeCharacterController"));
        PowerupIcon(0.5f, "speedPowerUpIcon").spawnInstance();

        characterController.setMaxSpeed(10.0f);
    }
} // fmwk