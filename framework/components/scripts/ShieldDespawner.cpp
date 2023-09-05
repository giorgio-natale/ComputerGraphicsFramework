//
// Created by Matilde on 23/08/2023.
//

#include <iostream>
#include "ShieldDespawner.h"
#include "../../Entity.h"
#include "../../GameEngine.h"

namespace fmwk {
    void ShieldDespawner::despawn() {
        auto gameEngine = fmwk::GameEngine::getInstance();
        auto&  health = gameEngine->getEntityByName("Character").getHealth();
        health.setShield(false);
        health.setTimeout(health.getGracePeriod());
        _parentEntity->markForRemoval();
    }
} // fmwk