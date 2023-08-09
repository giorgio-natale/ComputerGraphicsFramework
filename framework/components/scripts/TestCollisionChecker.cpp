//
// Created by Matilde on 07/08/2023.
//

#include "TestCollisionChecker.h"
#include "../../GameEngine.h"

namespace fmwk {
    TestCollisionChecker::TestCollisionChecker(const std::string &name) : Component(name) {}

    void TestCollisionChecker::update() {
        GameEngine *gameEngine = GameEngine::getInstance();
        std::vector<Entity *> collidingEntities = gameEngine->getCollidingEntities(
                &_parentEntity->getCollider(), nullptr);
        for (Entity *entity: collidingEntities) {
            std::cout << "COLLISION WITH " << entity->getName() << std::endl;
        }
    }

} // fmwk