//
// Created by drgio on 21/08/2023.
//

#include "Bullet.h"
#include "../GameEngine.h"
#include "../components/mesh/MeshComponent.h"
#include "../components/texture/TextureComponent.h"
#include "../components/materials/DefaultMaterial.h"
#include "../components/scripts/BulletAI.h"

namespace fmwk {
    void Bullet::buildEntity() {

        GameEngine *gameEngine = GameEngine::getInstance();

        auto bulletEntity = std::make_unique<fmwk::Entity>(
                "spawnedBullet" + std::to_string(getNewNumber()),
                _spawnPosition + _centerOffset,
                glm::quat(1,0,0,0)
        );
        bulletEntity->addComponent(std::make_unique<fmwk::MeshComponent>(gameEngine->getModelByName("mySphere")));
        bulletEntity->addComponent(
                std::make_unique<fmwk::TextureComponent>(_texture));
        bulletEntity->addComponent(
                std::make_unique<fmwk::DefaultMaterial>(2.5f));
        bulletEntity->addComponent(std::make_unique<fmwk::Collider>(0.195f, "BULLET", glm::vec3(0, 0, 0)));
        bulletEntity->addComponent(std::make_unique<fmwk::BulletAI>("bulletAI", _bulletSpeed, _direction, &_targetTags));

        gameEngine->enqueueEntity(std::move(bulletEntity));

    }

    Bullet::Bullet(const glm::vec3 &spawnPosition, const glm::vec3 &centerOffset, float bulletSpeed,
                   const glm::vec3 &direction, const std::unordered_set<std::string> &targetTags, BoundTexture& texture) : _spawnPosition(
            spawnPosition), _centerOffset(centerOffset), _bulletSpeed(bulletSpeed), _direction(direction), _targetTags(
            targetTags), _texture(texture){}
} // fmwk