//
// Created by Matilde on 22/08/2023.
//

#include <iostream>
#include "PowerUpSpawnBossEnemy.h"
#include "../../GameEngine.h"
#include "../../blueprints/BossEnemy.h"

namespace fmwk {
    void PowerUpSpawnBossEnemy::applyPowerUp() {
        auto gameEngine = fmwk::GameEngine::getInstance();
        if (!gameEngine->doesEntityExist("FinalBoss")) {
            auto &characterTransform = gameEngine->getEntityByName("Character").getTransform();
            BossEnemy(characterTransform).spawnInstance();
        }
    }
} // fmwk