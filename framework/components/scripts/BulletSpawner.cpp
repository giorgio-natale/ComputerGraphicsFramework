//
// Created by Matilde on 07/08/2023.
//

#include "BulletSpawner.h"
#include "../../GameEngine.h"
#include "../mesh/MeshComponent.h"
#include "../texture/TextureComponent.h"
#include "../materials/DefaultMaterial.h"
#include "BulletAI.h"

namespace fmwk {
    BulletSpawner::BulletSpawner(const std::string &name, const glm::vec3 &centerOffset, float bulletCoolDown,
                                 float bulletSpeed, std::unordered_set<std::string> const* targetTags) : Component(name),
                                    _centerOffset(centerOffset),
                                    _bulletCoolDown(bulletCoolDown), _bulletSpeed(bulletSpeed),
                                    _targetTags(*targetTags){
        _spawnEnabled = true;
        _timeFromLastSpawn = 0;
    }

    void BulletSpawner::update() {
        GameEngine *gameEngine = GameEngine::getInstance();
        _timeFromLastSpawn += gameEngine->getInput().deltaTime;

        if (_timeFromLastSpawn >= _bulletCoolDown) {
            _spawnEnabled = true;
        }

        if (gameEngine->getInput().spacePressed && _spawnEnabled) {
            spawnBullet(_bulletSpeed, -1.0f * (_parentEntity->getTransform().getLocalDirections()[2]));
            _timeFromLastSpawn = 0;
            _spawnEnabled = false;
        }

    }

    void BulletSpawner::spawnBullet(float bulletSpeed, glm::vec3 direction) {
        static int bulletId = 0;
        GameEngine *gameEngine = GameEngine::getInstance();
        auto &transform = _parentEntity->getTransform();

        auto bulletEntity = std::make_unique<fmwk::Entity>("spawnedBullet" + std::to_string(bulletId),
                                                           transform.getPosition(), transform.getRotation());
        bulletEntity->getTransform().setScale(glm::vec3(0.5f, 0.5f, 0.5f));
        bulletEntity->addComponent(std::make_unique<fmwk::MeshComponent>(gameEngine->getModelByName("mySphere")));
        bulletEntity->addComponent(
                std::make_unique<fmwk::TextureComponent>(gameEngine->getBoundTextureByName("sphereTexture")));
        bulletEntity->addComponent(
                std::make_unique<fmwk::DefaultMaterial>(2.5f));
        bulletEntity->addComponent(std::make_unique<fmwk::Collider>(0.5f, "BULLET"));
        bulletEntity->addComponent(std::make_unique<fmwk::BulletAI>("bulletAI", bulletSpeed, direction, &_targetTags));

        gameEngine->enqueueEntity(std::move(bulletEntity));
        bulletId++;
    }

} // fmwk