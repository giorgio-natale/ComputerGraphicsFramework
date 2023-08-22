//
// Created by Matilde on 07/08/2023.
//

#include "BulletSpawner.h"
#include "../../GameEngine.h"
#include "../mesh/MeshComponent.h"
#include "../texture/TextureComponent.h"
#include "../materials/DefaultMaterial.h"
#include "BulletAI.h"
#include "../../blueprints/Bullet.h"

namespace fmwk {
    BulletSpawner::BulletSpawner(const std::string &name, const glm::vec3 &centerOffset, float bulletSpeed,
                                 float bulletCoolDown, std::unordered_set<std::string> const *targetTags) : Component(
            name),
                                                                                                            _centerOffset(
                                                                                                                    centerOffset),
                                                                                                            _bulletCoolDown(
                                                                                                                    bulletCoolDown),
                                                                                                            _bulletSpeed(
                                                                                                                    bulletSpeed),
                                                                                                            _targetTags(
                                                                                                                    *targetTags) {
        _timeFromLastSpawn = 0;
    }

    void BulletSpawner::spawnBullet(float bulletSpeed, glm::vec3 direction, BoundTexture& texture) {
        Bullet( _parentEntity->getTransform().getPosition(), glm::vec3(0), bulletSpeed, direction, _targetTags, texture).spawnInstance();
    }

    void BulletSpawner::setBulletCoolDown(float bulletCoolDown) {
        _bulletCoolDown = bulletCoolDown;
    }

} // fmwk