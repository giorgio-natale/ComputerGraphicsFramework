//
// Created by Matilde on 11/08/2023.
//

#include "EnemyBulletSpawner.h"
#include "../../GameEngine.h"

namespace fmwk {
    EnemyBulletSpawner::EnemyBulletSpawner(const glm::vec3 &centerOffset, float bulletSpeed, float bulletCoolDown,
                                           Transform &characterTransform,
                                           std::unordered_set<std::string> const *targetTags) :
            BulletSpawner("EnemyBulletSpawner", centerOffset, bulletSpeed, bulletCoolDown, targetTags),
            _characterTransform(characterTransform) {}

    void EnemyBulletSpawner::update() {
        GameEngine *gameEngine = GameEngine::getInstance();
        _timeFromLastSpawn += gameEngine->getInput().deltaTime;

        if (_timeFromLastSpawn >= _bulletCoolDown) {
            glm::vec3 dir = _characterTransform.getPosition() - _parentEntity->getTransform().getPosition();
            if(dir == glm::vec3(0,0,0)) {
                dir = X;
            }
            spawnBullet(_bulletSpeed, glm::normalize(dir));
            _timeFromLastSpawn = 0;
        }

    }
} // fmwk