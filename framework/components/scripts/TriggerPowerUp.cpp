#include "TriggerPowerUp.h"
#include "../../GameEngine.h"
#include "PowerUp.h"

namespace fmwk {
    TriggerPowerUp::TriggerPowerUp() : Component("TriggerPowerUp") {}

    void TriggerPowerUp::update() {
        auto gameEngine = GameEngine::getInstance();
        Entity const * character = gameEngine->getCharacterCollidingEntity(&_parentEntity->getCollider());
        if(character != nullptr) {
            reinterpret_cast<PowerUp&>(_parentEntity->getComponentByName("PowerUp")).applyPowerUp();
            _parentEntity->markForRemoval();
        }
    }
} // fmwk