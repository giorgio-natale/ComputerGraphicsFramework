//
// Created by Matilde on 22/08/2023.
//

#include <iostream>
#include "PowerUpDecreaseBulletCoolDown.h"
#include "../../GameEngine.h"
#include "CharacterBulletSpawner.h"
#include "../../blueprints/PowerupIcon.h"

namespace fmwk {
    void PowerUpDecreaseBulletCoolDown::applyPowerUp() {
        std::cout << "POWER UP DECREASE BULLET COOL-DOWN" << std::endl;
        auto gameEngine = fmwk::GameEngine::getInstance();
        auto&  bulletSpawner = reinterpret_cast<fmwk::CharacterBulletSpawner&>(gameEngine->getEntityByName("Character").getComponentByName("CharacterBulletSpawner"));
        PowerupIcon(1.5f, "bulletPowerUpIcon").spawnInstance();
        bulletSpawner.setBulletCoolDown(0.3f);
    }
} // fmwk