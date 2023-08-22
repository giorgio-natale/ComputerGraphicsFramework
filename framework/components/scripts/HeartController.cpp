//
// Created by drgio on 21/08/2023.
//

#include "HeartController.h"
#include "../../GameEngine.h"

namespace fmwk {
    void HeartController::postUpdate() {
        auto gameEngine = GameEngine::getInstance();
        if(gameEngine->doesEntityExist("Character")){
            auto&  health = gameEngine->getEntityByName("Character").getHealth();
            if(health.getCurrentLifePercentage() > (float)(_index) / 6.0f)
                _parentEntity->setVisible(true);
            else
                _parentEntity->setVisible(false);
        }

    }

    HeartController::HeartController(int index) : Component("HeartController"), _index(index) {}
} // fmwk