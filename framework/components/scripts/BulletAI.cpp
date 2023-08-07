//
// Created by Matilde on 07/08/2023.
//

#include "BulletAI.h"
#include "../../GameEngine.h"

namespace fmwk {
    BulletAI::BulletAI(const std::string &name, float bulletSpeed, const glm::vec3 &direction,
                       std::unordered_set<std::string> const *targetTags) : Component(name),
                                                                            _bulletSpeed(
                                                                                    bulletSpeed),
                                                                            _direction(
                                                                                    direction),
                                                                            _targetTags(*targetTags) {}

    void BulletAI::update() {
        auto gameEngine = GameEngine::getInstance();
        Transform &transform = _parentEntity->getTransform();

        float deltaT = gameEngine->getInput().deltaTime;
        glm::vec3 deltaPosition = deltaT * _bulletSpeed * _direction;
        transform.translate(deltaPosition);

        std::vector<Entity *> collidingEntities = gameEngine->getCollidingEntities(
                static_cast<Collider *>(&(_parentEntity->getComponentByName("bulletCollider"))),
                &_targetTags);
        for (Entity *entity: collidingEntities) {
            std::cout << "BULLET " << _parentEntity->getName() << " COLLIDED WITH " << entity->getName() << std::endl;
        }
    }
} // fmwk