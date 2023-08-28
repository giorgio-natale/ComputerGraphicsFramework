//
// Created by Matilde on 22/08/2023.
//

#include "PowerUpCube.h"
#include "../GameEngine.h"
#include "../components/mesh/MeshComponent.h"
#include "../components/texture/TextureComponent.h"
#include "../components/scripts/TriggerPowerUp.h"
#include "../components/scripts/PowerUpAddLife.h"
#include "../components/scripts/PowerUpSetShield.h"
#include "../components/scripts/PowerUpIncreaseSpeed.h"
#include "../components/scripts/PowerUpDecreaseBulletCoolDown.h"
#include "../components/scripts/PowerUpSpawnBossEnemy.h"
#include "../components/materials/SimplePhongMaterial.h"

namespace fmwk {
    PowerUpCube::PowerUpCube(const glm::vec3 &position, PowerUpType type) : _position(position),
                                                                            _type(type) {}

    void PowerUpCube::buildEntity() {
        auto gameEngine = GameEngine::getInstance();
        auto powerUpEntity = std::make_unique<fmwk::Entity>("powerUpEntity" + std::to_string(getNewNumber()), _position,
                                                            glm::quat(1, 0, 0, 0));
        powerUpEntity->addComponent(std::make_unique<fmwk::MeshComponent>(gameEngine->getModelByName("myCube")));
        powerUpEntity->getTransform().setScale(glm::vec3(0.8f, 0.8f, 0.8f));
        powerUpEntity->addComponent(std::make_unique<fmwk::SimplePhongMaterial>());
        powerUpEntity->addComponent(std::make_unique<fmwk::TriggerPowerUp>());
        switch (_type) {
            case SET_SHIELD:
                powerUpEntity->addComponent(std::make_unique<fmwk::Collider>(1.0f, "POWER_UP", glm::vec3(0, 0, 0)));
                powerUpEntity->addComponent(std::make_unique<fmwk::TextureComponent>(gameEngine->getBoundTextureByName("powerUpShield")));
                powerUpEntity->addComponent(std::make_unique<fmwk::PowerUpSetShield>());
                break;
            case INCREASE_SPEED:
                powerUpEntity->addComponent(std::make_unique<fmwk::Collider>(1.0f, "POWER_UP", glm::vec3(0, 0, 0)));
                powerUpEntity->addComponent(std::make_unique<fmwk::TextureComponent>(gameEngine->getBoundTextureByName("powerUpSpeedUp")));
                powerUpEntity->addComponent(std::make_unique<fmwk::PowerUpIncreaseSpeed>());
                break;
            case DECREASE_BULLET_COOL_DOWN:
                powerUpEntity->addComponent(std::make_unique<fmwk::Collider>(1.0f, "POWER_UP", glm::vec3(0, 0, 0)));
                powerUpEntity->addComponent(std::make_unique<fmwk::TextureComponent>(gameEngine->getBoundTextureByName("powerUpBullet")));
                powerUpEntity->addComponent(std::make_unique<fmwk::PowerUpDecreaseBulletCoolDown>());
                break;
            case ADD_LIFE:
                powerUpEntity->addComponent(std::make_unique<fmwk::Collider>(1.0f, "POWER_UP", glm::vec3(0, 0, 0)));
                powerUpEntity->addComponent(std::make_unique<fmwk::TextureComponent>(gameEngine->getBoundTextureByName("powerUpLife")));
                powerUpEntity->addComponent(std::make_unique<fmwk::PowerUpAddLife>());
                break;
            case SPAWN_BOSS_ENEMY:
                powerUpEntity->addComponent(std::make_unique<fmwk::Collider>(3.0f, "POWER_UP", glm::vec3(0, 0, 0)));
                powerUpEntity->addComponent(std::make_unique<fmwk::TextureComponent>(gameEngine->getBoundTextureByName("white")));
                powerUpEntity->addComponent(std::make_unique<fmwk::PowerUpSpawnBossEnemy>());
                powerUpEntity->getTransform().setScale(glm::vec3(6));
                powerUpEntity->setVisible(false);
                break;
        }

        gameEngine->enqueueEntity(std::move(powerUpEntity));
    }
} // fmwk