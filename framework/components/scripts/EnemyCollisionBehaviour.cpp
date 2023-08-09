//
// Created by drgio on 09/08/2023.
//

#include "EnemyCollisionBehaviour.h"
#include "../../GameEngine.h"

namespace fmwk {
    EnemyCollisionBehaviour::EnemyCollisionBehaviour() : Component("EnemyCollisionBehaviour") {}

    void EnemyCollisionBehaviour::update() {
        auto gameEngine = GameEngine::getInstance();
        Entity * character = gameEngine->getCharacterCollidingEntity(&_parentEntity->getCollider());
        if(character != nullptr)
            std::cout << "ENEMY " << _parentEntity->getName() << " COLLIDED WITH " << character->getName() << std::endl;
    }
} // fmwk