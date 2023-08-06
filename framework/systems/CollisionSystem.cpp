//
// Created by drgio on 04/08/2023.
//

#include "CollisionSystem.h"
#include "../Entity.h"

namespace fmwk {
    void CollisionSystem::addCollider(Collider *collider) {
        std::string key = collider->getParent()->getName() + "-" + collider->getName();
        if(_colliders.find(key) != _colliders.end())
            throw std::runtime_error("Collider " + key + " already added to the collision system");
        _colliders.insert({key, collider});
    }

    void CollisionSystem::removeCollider(const std::string &entityName, const std::string &colliderName) {
        std::string key = entityName + "-" + colliderName;
        auto elem = _colliders.find(entityName + "-" + colliderName);
        if(elem != _colliders.end()){
            _colliders.erase(key);
        }
    }
} // fmwk