//
// Created by drgio on 21/08/2023.
//

#ifndef DEMO_BULLET_H
#define DEMO_BULLET_H

#include "Blueprint.h"
#include "../systems/TextureSystem.h"

namespace fmwk {

    class Bullet : public Blueprint {
    public:
        Bullet(const glm::vec3 &spawnPosition, const glm::vec3 &centerOffset, float bulletSpeed,
               const glm::vec3 &direction, const std::unordered_set<std::string> &targetTags, BoundTexture &texture);

    protected:
        void buildEntity() override;
    private:
        glm::vec3 _spawnPosition;
        glm::vec3 _centerOffset;
        float _bulletSpeed;
        glm::vec3 _direction;
        std::unordered_set<std::string> _targetTags;
        BoundTexture &_texture;

    };

} // fmwk

#endif //DEMO_BULLET_H
