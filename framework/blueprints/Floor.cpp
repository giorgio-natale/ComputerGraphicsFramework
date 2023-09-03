//
// Created by drgio on 24/08/2023.
//

#include "Floor.h"
#include "../GameEngine.h"
#include "../components/mesh/MeshComponent.h"
#include "../components/texture/TextureComponent.h"
#include "../components/materials/GGXMaterial.h"
#include "../components/materials/GGXNoNormalMaterial.h"

namespace fmwk {
    Floor::Floor(float width, float height) : _width(width), _height(height) {}

    void Floor::buildEntity() {
        auto gameEngine = GameEngine::getInstance();
        std::vector<VertexWithNormal> floorVertices{};
        std::vector<uint32_t> indices{};

        floorVertices.push_back({{0,0,0}, {0, _height}, fmwk::Y}); //00
        floorVertices.push_back({{_width,0,0}, {_width, _height}, fmwk::Y}); //10
        floorVertices.push_back({{_width,0,-_height}, {_width, 0},fmwk::Y}); //11
        floorVertices.push_back({{0,0,-_height}, {0, 0},fmwk::Y}); //11

        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(2);
        indices.push_back(2);
        indices.push_back(3);
        indices.push_back(0);
        gameEngine->addModel<fmwk::VertexWithNormal>("FloorModel", VERTEX_WITH_NORMAL, floorVertices, indices, glm::vec3(0), glm::quat(1,0,0,0), glm::vec3(1));

        auto floorEntity = std::make_unique<fmwk::Entity>("Floor");
        floorEntity->addComponent(std::make_unique<fmwk::MeshComponent>(gameEngine->getModelByName("FloorModel")));
        floorEntity->addComponent(std::make_unique<fmwk::TextureComponent>(gameEngine->getBoundTextureByName("rockColor")));
        floorEntity->addComponent(std::make_unique<fmwk::GGXNoNormalMaterial>(gameEngine->getBoundTextureByName("rockRoughness").getTexture()));

        gameEngine->enqueueEntity(std::move(floorEntity));
    }

} // fmwk