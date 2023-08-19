//
// Created by drgio on 19/08/2023.
//

#include <memory>
#include "RoughSphere.h"
#include "../Entity.h"
#include "../components/mesh/MeshComponent.h"
#include "../components/texture/TextureComponent.h"
#include "../components/materials/GGXMaterial.h"
#include "../components/scripts/EntityDespawner.h"
#include "../GameEngine.h"

namespace fmwk {
    RoughSphere::RoughSphere(const glm::vec3 &position, const glm::quat &rotation) : _position(position),
                                                                                     _rotation(rotation) {}
    void RoughSphere::buildEntity() {
        auto gameEngine = GameEngine::getInstance();
        auto sphereEntity = std::make_unique<fmwk::Entity>("spawnedSphere" + std::to_string(getNewNumber()), _position, _rotation);
        //sphereEntity->getTransform().setScale(glm::vec3(0.5f, 0.5f, 0.5f));
        sphereEntity->addComponent(std::make_unique<fmwk::Collider>(1.0f, "BULLET", glm::vec3(0,0,0)));
        sphereEntity->addComponent(std::make_unique<fmwk::MeshComponent>(gameEngine->getModelByName("mySphere")));
        sphereEntity->addComponent(std::make_unique<fmwk::TextureComponent>(gameEngine->getBoundTextureByName("sphereTexture")));
        sphereEntity->addComponent(std::make_unique<fmwk::GGXMaterial>(gameEngine->getBoundTextureByName("sphereNormal").getTexture(), gameEngine->getBoundTextureByName("sphereMaterial").getTexture()));
        sphereEntity->addComponent(std::make_unique<fmwk::EntityDespawner>("Despawner", 6.0f));

        gameEngine->enqueueEntity(std::move(sphereEntity));
    }


} // fmwk