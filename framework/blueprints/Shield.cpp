//
// Created by Matilde on 22/08/2023.
//

#include "Shield.h"
#include "../GameEngine.h"
#include "../components/mesh/MeshComponent.h"
#include "../components/texture/TextureComponent.h"
#include "../components/materials/GGXMaterial.h"
#include "../components/scripts/EntityDespawner.h"
#include "../components/materials/DefaultTransparent.h"
#include "../components/scripts/FollowEntity.h"
#include "../components/scripts/ShieldDespawner.h"

namespace fmwk {
    void Shield::buildEntity() {
        auto gameEngine = GameEngine::getInstance();
        auto &characterTransform = gameEngine->getEntityByName("Character").getTransform();

        auto shieldEntity = std::make_unique<fmwk::Entity>("Shield",
                                                           characterTransform.getPosition(), glm::quat(1, 0, 0, 0));
        shieldEntity->addComponent(std::make_unique<fmwk::Collider>(2.0f, "CHARACTER", glm::vec3(0, 0, 0)));
        shieldEntity->addComponent(std::make_unique<fmwk::MeshComponent>(gameEngine->getModelByName("shieldSphere")));
        shieldEntity->addComponent(
                std::make_unique<fmwk::TextureComponent>(gameEngine->getBoundTextureByName("sphereTexture")));
        shieldEntity->addComponent(std::make_unique<fmwk::DefaultTransparent>(1.0f, 0.8f));
        shieldEntity->addComponent(std::make_unique<fmwk::Health>(10.0f,2.0f));
        shieldEntity->addComponent(std::make_unique<fmwk::FollowEntity>(characterTransform));
        shieldEntity->addComponent(std::make_unique<fmwk::ShieldDespawner>());

        shieldEntity->setPreferredRenderOrder(50);
        gameEngine->enqueueEntity(std::move(shieldEntity));
    }
} // fmwk