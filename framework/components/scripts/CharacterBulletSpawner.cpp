//
// Created by Matilde on 11/08/2023.
//

#include "CharacterBulletSpawner.h"
#include "../../GameEngine.h"

namespace fmwk {
    CharacterBulletSpawner::CharacterBulletSpawner(const glm::vec3 &centerOffset, float bulletSpeed,
                                                   float bulletCoolDown,
                                                   std::unordered_set<std::string> const *targetTags) :
            BulletSpawner("CharacterBulletSpawner", centerOffset, bulletSpeed, bulletCoolDown, targetTags) {
        _spawnEnabled = true;
    }

    void CharacterBulletSpawner::update() {
        GameEngine *gameEngine = GameEngine::getInstance();
        _timeFromLastSpawn += gameEngine->getInput().deltaTime;

        if (_timeFromLastSpawn >= _bulletCoolDown) {
            _spawnEnabled = true;
        }

        if (gameEngine->getInput().spacePressed && _spawnEnabled) {
            spawnBullet(_bulletSpeed, -1.0f * (_parentEntity->getTransform().getLocalDirections()[2]), gameEngine->getBoundTextureByName("red"));
            _timeFromLastSpawn = 0;
            _spawnEnabled = false;
        }
    }
} // fmwk