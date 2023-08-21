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
#include "../components/materials/SimplePhongColorBlendMaterial.h"

namespace fmwk {
    void BasicEnemy::buildEntity() {
        auto gameEngine = GameEngine::getInstance();
        auto enemyEntity = std::make_unique<fmwk::Entity>("Enemy" + std::to_string(getNewNumber()), _targetPoints[0], glm::quat(1,0,0,0));
        enemyEntity->addComponent(std::make_unique<fmwk::Health>(50.0f,1.0f));
        enemyEntity->addComponent(std::make_unique<fmwk::Collider>(1.0f, "ENEMY", glm::vec3(0,0,0)));
        enemyEntity->addComponent(std::make_unique<fmwk::MeshComponent>(gameEngine->getModelByName("ghost")));
        enemyEntity->addComponent(std::make_unique<fmwk::TextureComponent>(gameEngine->getBoundTextureByName("dungeonTexture")));
        enemyEntity->addComponent(std::make_unique<fmwk::SimplePhongColorBlendMaterial>(glm::vec3(1,0,0)));
        enemyEntity->addComponent(std::make_unique<fmwk::EnemyController>(_targetPoints, 6.0f, 6.0f, 0.4f));
        enemyEntity->addComponent(std::make_unique<fmwk::EnemyCollisionBehaviour>());
        gameEngine->enqueueEntity(std::move(enemyEntity));


    }

    BasicEnemy::BasicEnemy(const std::vector<glm::vec3> &targetPoints) : _targetPoints(targetPoints) {}
} // fmwk