//
// Created by Matilde on 22/08/2023.
//

#include <iostream>
#include "PowerUpDecreaseBulletCoolDown.h"
#include "../../GameEngine.h"
#include "CharacterBulletSpawner.h"

namespace fmwk {
    PowerUpDecreaseBulletCoolDown::PowerUpDecreaseBulletCoolDown() {}

    void PowerUpDecreaseBulletCoolDown::applyPowerUp() {
        std::cout << "POWER UP DECREASE BULLET COOL-DOWN" << std::endl;
        auto gameEngine = fmwk::GameEngine::getInstance();
        auto&  bulletSpawner = reinterpret_cast<fmwk::CharacterBulletSpawner&>(gameEngine->getEntityByName("Character").getComponentByName("CharacterBulletSpawner"));
        bulletSpawner.setBulletCoolDown(0.3f);
    }
} // fmwk