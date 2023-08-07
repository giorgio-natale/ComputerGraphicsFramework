//
// Created by Matilde on 07/08/2023.
//

#ifndef DEMO_BULLETSPAWNER_H
#define DEMO_BULLETSPAWNER_H

#include <glm/vec3.hpp>
#include "Component.h"

namespace fmwk {

    class BulletSpawner: public Component{
    public:
        BulletSpawner(const std::string &name, const glm::vec3 &centerOffset, float bulletCoolDown, float bulletSpeed);

        void update() override;

    private:
        glm::vec3 _centerOffset;
        float _bulletCoolDown;
        float _bulletSpeed;
        bool _spawnEnabled;
        float _timeFromLastSpawn;
        void spawnBullet(float bulletSpeed, glm::vec3 direction);
    };

} // fmwk

#endif //DEMO_BULLETSPAWNER_H
