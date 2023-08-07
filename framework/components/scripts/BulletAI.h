//
// Created by Matilde on 07/08/2023.
//

#ifndef DEMO_BULLETAI_H
#define DEMO_BULLETAI_H

#include <glm/vec3.hpp>
#include <unordered_set>
#include "Component.h"

namespace fmwk {

    class BulletAI : public Component {
    public:
        BulletAI(const std::string &name, float bulletSpeed, const glm::vec3 &direction,
                 std::unordered_set<std::string> const *targetTags);

        void update() override;

    private:
        float _bulletSpeed;
        glm::vec3 _direction;
        std::unordered_set<std::string> _targetTags;
    };

} // fmwk

#endif //DEMO_BULLETAI_H
