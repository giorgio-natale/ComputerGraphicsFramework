//
// Created by drgio on 04/08/2023.
//

#include "EntityDespawner.h"
#include "../../GameEngine.h"

namespace fmwk {
    EntityDespawner::EntityDespawner(const std::string &name, float delay) : Component(name), _delay(delay), _timeElapsed(0) {}

    void EntityDespawner::update() {
        auto gameEngine = GameEngine::getInstance();

        _timeElapsed += gameEngine->getInput().deltaTime;
        if(_timeElapsed >= _delay){
            //gameEngine->enqueueEntityRemoval(_parentEntity->getName());
            _parentEntity->enqueueComponentRemoval("Collider");
            _parentEntity->enqueueComponentRemoval(_name);
        }
    }
} // fmwk