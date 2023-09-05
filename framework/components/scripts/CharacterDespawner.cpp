//
// Created by Matilde on 23/08/2023.
//

#include <iostream>
#include "CharacterDespawner.h"
#include "../../GameEngine.h"
#include "../camera/PerspectiveCamera.h"
#include "../../blueprints/SplashScreen.h"

namespace fmwk {
    void CharacterDespawner::despawn() {
        auto gameEngine = GameEngine::getInstance();
        gameEngine->enqueueRemovalOfAllEntities();
        auto cameraEntity = std::make_unique<fmwk::Entity>("Camera", glm::vec3(0, 0, 10), glm::quat(1, 0, 0, 0));
        auto cameraComponent = std::make_unique<fmwk::PerspectiveCamera>(0.1f, 60.0f, glm::radians(45.0f));
        cameraEntity->addComponent(std::move(cameraComponent));
        gameEngine->enqueueEntity(std::move(cameraEntity));
        SplashScreen("gameOver").spawnInstance();
    }
} // fmwk