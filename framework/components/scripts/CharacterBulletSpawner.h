//
// Created by Matilde on 11/08/2023.
//

#ifndef DEMO_CHARACTERBULLETSPAWNER_H
#define DEMO_CHARACTERBULLETSPAWNER_H

#include "BulletSpawner.h"

namespace fmwk {

    class CharacterBulletSpawner : public BulletSpawner {
    public:
        CharacterBulletSpawner(const glm::vec3 &centerOffset,
                               float bulletSpeed, float bulletCoolDown,
                               std::unordered_set<std::string> const *targetTags);

        void update() override;

    private:
        bool _spawnEnabled;

    };

} // fmwk

#endif //DEMO_CHARACTERBULLETSPAWNER_H
