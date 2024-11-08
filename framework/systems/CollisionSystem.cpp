#include "CollisionSystem.h"
#include "../Entity.h"
#include "../GameEngine.h"

namespace fmwk {
    void CollisionSystem::addCollider(Collider *collider) {
        std::string key = collider->getParent()->getName() + "-" + collider->getName();
        if (_colliders.find(key) != _colliders.end())
            throw std::runtime_error("Collider " + key + " already added to the collision system");
        _colliders.insert({key, collider});
    }

    void CollisionSystem::removeCollider(const std::string &entityName, const std::string &colliderName) {
        std::string key = entityName + "-" + colliderName;
        auto elem = _colliders.find(entityName + "-" + colliderName);
        if (elem != _colliders.end()) {
            _colliders.erase(key);
        }
    }

    bool CollisionSystem::checkCollision(Collider *colliderA, Collider *colliderB) {
        glm::vec3 centerA = colliderA->getParent()->getTransform().getPosition() + colliderA->getCenterOffset();
        glm::vec3 centerB = colliderB->getParent()->getTransform().getPosition() + colliderB->getCenterOffset();
        return glm::distance(centerA, centerB) < colliderA->getRadius() + colliderB->getRadius();
    }

    std::vector<Entity *>
    CollisionSystem::getCollidingEntities(Collider *collider, std::unordered_set<std::string> const *targetTags) {
        GameEngine *gameEngine = GameEngine::getInstance();
        std::string selfEntityName = collider->getParent()->getName();

        std::vector<Entity *> collidingEntities;
        for (auto const &[key, otherCollider]: _colliders) {
            std::string collidingEntityName = key.substr(0, key.find('-'));
            if (selfEntityName != collidingEntityName && checkCollision(collider, otherCollider) &&
                (targetTags == nullptr || targetTags->find(otherCollider->getTag()) != targetTags->end())) {
                collidingEntities.push_back(&gameEngine->getEntityByName(collidingEntityName));
            }
        }
        return collidingEntities;
    }

    Entity const *CollisionSystem::getCharacterCollidingEntity(Collider *collider) {
        auto elem = _colliders.find("Shield-Collider");
        if (elem == _colliders.end()) {
            elem = _colliders.find("Character-Collider");
        }

        if (elem != _colliders.end()) {
            Collider *characterCollider = elem->second;
            if (checkCollision(collider, characterCollider))
                return characterCollider->getParent();
        }
        return nullptr;
    }

} // fmwk