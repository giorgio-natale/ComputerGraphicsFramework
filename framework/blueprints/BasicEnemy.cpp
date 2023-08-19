//
// Created by drgio on 19/08/2023.
//

#include "BasicEnemy.h"
#include "../GameEngine.h"
#include "../components/mesh/MeshComponent.h"
#include "../components/texture/TextureComponent.h"
#include "../components/materials/DefaultMaterial.h"
#include "../components/scripts/EnemyController.h"
#include "../components/scripts/EnemyCollisionBehaviour.h"

namespace fmwk {
    void BasicEnemy::buildEntity() {
        auto gameEngine = GameEngine::getInstance();
        auto enemyEntity = std::make_unique<fmwk::Entity>("Enemy" + std::to_string(getNewNumber()), _targetPoints[0], glm::quat(1,0,0,0));
        //enemyEntity->getTransform().setScale({0.5, 0.5, 0.5});
        enemyEntity->addComponent(std::make_unique<fmwk::Health>(50.0f,1.0f));
        enemyEntity->addComponent(std::make_unique<fmwk::Collider>(1.0f, "ENEMY", glm::vec3(0,0,0)));
        enemyEntity->addComponent(std::make_unique<fmwk::MeshComponent>(gameEngine->getModelByName("mySphere")));
        enemyEntity->addComponent(std::make_unique<fmwk::TextureComponent>(gameEngine->getBoundTextureByName("sphereTexture")));
        enemyEntity->addComponent(std::make_unique<fmwk::DefaultMaterial>(1.0f));
        enemyEntity->addComponent(std::make_unique<fmwk::EnemyController>(_targetPoints, 2.0f, 6.0f, 1.0f));
        enemyEntity->addComponent(std::make_unique<fmwk::EnemyCollisionBehaviour>());
        gameEngine->enqueueEntity(std::move(enemyEntity));


    }

    BasicEnemy::BasicEnemy(const std::vector<glm::vec3> &targetPoints) : _targetPoints(targetPoints) {}
} // fmwk