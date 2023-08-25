//
// Created by drgio on 21/08/2023.
//

#include "BossEnemy.h"
#include "../components/mesh/MeshComponent.h"
#include "../components/texture/TextureComponent.h"
#include "../components/materials/DefaultMaterial.h"
#include "../components/scripts/FinalBossController.h"
#include "../GameEngine.h"
#include "../components/materials/SimplePhongColorBlendMaterial.h"
#include "../components/scripts/EnemyBulletSpawner.h"
#include "../components/scripts/EnemyCollisionBehaviour.h"
#include "../components/materials/GGXMaterial.h"
#include "BossBarUI.h"
#include "../components/scripts/BossEnemyDespawner.h"

namespace fmwk {
    BossEnemy::BossEnemy(Transform &characterTransform) : _characterTransform(characterTransform) {}

    void BossEnemy::buildEntity() {

        auto gameEngine = GameEngine::getInstance();
        auto boss = std::make_unique<fmwk::Entity>("FinalBoss");

        boss->addComponent(std::make_unique<fmwk::MeshComponent>(gameEngine->getModelByName("death")));
        auto bossHealth = std::make_unique<fmwk::Health>(100.0f,1.0f);
        BossBarUI(*bossHealth.get()).spawnInstance();
        boss->addComponent(std::move(bossHealth));
        boss->addComponent(std::make_unique<fmwk::Collider>(1.0f, "ENEMY", glm::vec3(0,0,0)));
        boss->addComponent(std::make_unique<fmwk::TextureComponent>(gameEngine->getBoundTextureByName("death")));
        boss->addComponent(std::make_unique<fmwk::SimplePhongColorBlendMaterial>(glm::vec3(1,0,0)));

        boss->addComponent(std::make_unique<fmwk::FinalBossController>(glm::vec3(108.0f,0.5f,-102.0f), _characterTransform));
        std::unordered_set<std::string> targetTags = std::unordered_set<std::string>({"CHARACTER"});
        boss->addComponent(std::make_unique<EnemyBulletSpawner>(glm::vec3(0,0,0), 12.0f, 1.5f, _characterTransform, &targetTags));
        boss->addComponent(std::make_unique<fmwk::EnemyCollisionBehaviour>());
        boss->addComponent(std::make_unique<fmwk::BossEnemyDespawner>());

        gameEngine->enqueueEntity(std::move(boss));

    }

} // fmwk