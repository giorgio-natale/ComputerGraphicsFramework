//
// Created by drgio on 18/07/2023.
//

#include "ModelSystem.h"

namespace fmwk {
     ModelSystem::ModelSystem(BaseProject *bp){
        _bp = bp;
        VertexDescriptor vd;

        vd.init(
                _bp,
                {
                        {0, sizeof(Vertex), VK_VERTEX_INPUT_RATE_VERTEX}
                   },
                {
                        {0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, pos),sizeof(glm::vec3), POSITION},
                        {0, 1, VK_FORMAT_R32G32_SFLOAT,    offsetof(Vertex, UV),sizeof(glm::vec2), UV}
                   }
        );

        _vertexDescriptors.insert({VERTEX, {vd, "shaders/ShaderVert.spv"}});


        vd.init(
            _bp,
            {
                    {0, sizeof(VertexWithNormal), VK_VERTEX_INPUT_RATE_VERTEX}
               },
            {
                    {0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VertexWithNormal, pos),sizeof(glm::vec3), POSITION},
                    {0, 1, VK_FORMAT_R32G32_SFLOAT,    offsetof(VertexWithNormal, UV),sizeof(glm::vec2), UV},
                    {0, 2, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VertexWithNormal, norm),sizeof(glm::vec3), NORMAL},
               }
        );

        _vertexDescriptors.insert({VERTEX, {vd, "shaders/ShaderVert.spv"}});

    }

    void ModelSystem::addModel(std::string const& name, VertexType vertexType, std::string const& fileName){
        if(_models.find(name) != _models.end())
            throw std::runtime_error("Could not add model with name '" + name + "' because there was another model with the same name");
        auto extension = fileName.substr(fileName.find_last_of('.') + 1);
        ModelType modelType;
        if(extension == "gltf" || extension == "glb") {
            modelType = GLTF;
        } else if(extension == "obj") {
            modelType = OBJ;
        } else {
            throw std::runtime_error("Cannot recognize model extension '" + extension + "'");
        }

        std::unique_ptr<BaseModel> model;
        switch (vertexType) {
            case VERTEX: {
                model = std::make_unique<Model<Vertex>>();
                break;
            }
            case VERTEX_WITH_NORMAL:
                model = std::make_unique<Model<VertexWithNormal>>();
                break;
        }

        model->init(_bp, &_vertexDescriptors.find(vertexType)->second.first, fileName, modelType);
        _models.insert({name, TModel(std::move(model), vertexType)});

    }

    TModel& ModelSystem::getModelByName(std::string const& name){
        auto itr = _models.find(name);
        if (itr == _models.end())
            throw std::runtime_error("Could not find model with name '" + name + "'");
        return _models.find(name)->second;
    }

    std::unordered_map<VertexType, std::pair<VertexDescriptor, std::string>>& ModelSystem::getAllVertexDescriptors(){
        return _vertexDescriptors;
    }

    TModel::TModel(std::unique_ptr<BaseModel> model, VertexType type) {
        _model = std::move(model);
        _type = type;
    }

    VertexType TModel::getType() {
        return _type;
    }

    BaseModel &TModel::getTypedModel() {
        return *_model;
    }
} // fmwk