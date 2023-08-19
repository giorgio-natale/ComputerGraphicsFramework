//
// Created by drgio on 01/08/2023.
//

#include "Blueprint.h"
#include "../GameEngine.h"

namespace fmwk {
    int Blueprint::_uniqueNumber = 0;

    void Blueprint::spawnInstance() {
        buildEntity();
    }

    void Blueprint::addInstance() {
        buildEntity();
        GameEngine::getInstance()->flushEnqueuedEntityOperations();
    }

    int Blueprint::getNewNumber() {
        return _uniqueNumber++;
    }

    Entity *Blueprint::getMainEntity() {
        return nullptr;
    }
} // fmwk