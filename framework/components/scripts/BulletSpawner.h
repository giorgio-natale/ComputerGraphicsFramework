//
// Created by Matilde on 07/08/2023.
//

#ifndef DEMO_BULLETSPAWNER_H
#define DEMO_BULLETSPAWNER_H

#include <glm/vec3.hpp>
#include <unordered_set>
#include "Component.h"

namespace fmwk {

    class BulletSpawner : public Component {
    public:
        BulletSpawner(const std::string &name, const glm::vec3 &centerOffset, float bulletSpeed, float bulletCoolDown,
                      std::unordered_set<std::string> const *targetTags);

    protected:
        glm::vec3 _centerOffset;
        std::unordered_set<std::string> _targetTags;
        float _bulletSpeed;
        float _bulletCoolDown;
        float _timeFromLastSpawn;

        void spawnBullet(float bulletSpeed, glm::vec3 direction);
    };

} // fmwk

#endif //DEMO_BULLETSPAWNER_H
