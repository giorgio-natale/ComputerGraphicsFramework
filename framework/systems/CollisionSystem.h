//
// Created by drgio on 04/08/2023.
//

#ifndef DEMO_COLLISIONSYSTEM_H
#define DEMO_COLLISIONSYSTEM_H

#include <map>
#include <string>
#include "../components/collision/Collider.h"

namespace fmwk {

    class CollisionSystem {
    public:
        void addCollider(Collider* collider);
        void removeCollider(std::string const& entityName, std::string const& colliderName);

    private:
        std::map<std::string, Collider*> _colliders;
    };

} // fmwk

#endif //DEMO_COLLISIONSYSTEM_H
