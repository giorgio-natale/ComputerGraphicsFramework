//
// Created by Matilde on 11/08/2023.
//

#ifndef DEMO_ENEMYBULLETSPAWNER_H
#define DEMO_ENEMYBULLETSPAWNER_H

#include "BulletSpawner.h"
#include "../transform/Transform.h"

namespace fmwk {

    class EnemyBulletSpawner : public BulletSpawner {
    public:
        EnemyBulletSpawner(const glm::vec3 &centerOffset, float bulletSpeed, float bulletCoolDown,
                           Transform &characterTransform, std::unordered_set<std::string> const *targetTags);

        void update() override;

    private:
        Transform &_characterTransform;
    };

} // fmwk

#endif //DEMO_ENEMYBULLETSPAWNER_H
