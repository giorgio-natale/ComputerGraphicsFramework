//
// Created by drgio on 24/07/2023.
//

#include "CubeSpawner.h"
#include "../../GameEngine.h"
#include "../mesh/MeshComponent.h"
#include "../texture/TextureComponent.h"
#include "../materials/DefaultMaterial.h"
#include "../materials/ColorBlendComponent.h"
#include "../materials/SimplePhongMaterial.h"
#include "../materials/GGXMaterial.h"
#include "EntityDespawner.h"
#include "../../blueprints/RoughSphere.h"

namespace fmwk {
    CubeSpawner::CubeSpawner(const std::string &name) : Component(name) {}

    void CubeSpawner::update() {
        auto gameEngine = GameEngine::getInstance();
        if(!gameEngine->getInput().spacePressed)
            _spawnEnabled = true;
        if(gameEngine->getInput().spacePressed && _spawnEnabled) {
            _spawnEnabled = false;
            auto &transform = _parentEntity->getTransform();
            RoughSphere(transform.getPosition(), transform.getRotation()).spawnInstance();
        }
    }
} // fmwk