//
// Created by Matilde on 22/08/2023.
//

#include <iostream>
#include "PowerUpIncreaseSpeed.h"
#include "../../GameEngine.h"
#include "MazeCharacterController.h"

namespace fmwk {
    void PowerUpIncreaseSpeed::applyPowerUp() {
        std::cout << "POWER UP INCREASE SPEED" << std::endl;
        auto gameEngine = fmwk::GameEngine::getInstance();
        auto&  characterController = reinterpret_cast<fmwk::MazeCharacterController&>(gameEngine->getEntityByName("Character").getComponentByName("MazeCharacterController"));
        characterController.setMaxSpeed(10.0f);
    }
} // fmwk