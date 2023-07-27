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

        std::set<VertexShader> defaultShaders = {
                {"shaders/PosUvToUv.spv", {FRAG_UV}},
                {"shaders/PosUvToUvFragPos.spv", {FRAG_UV, FRAG_POSITION}}
        };

        _vertexDescriptors.insert({VERTEX, {vd, defaultShaders}});


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

         std::set<VertexShader> posUvNormShaders = {
                 {"shaders/PosUvNormToUv.spv", {FRAG_UV}},
                 {"shaders/PosUvNormToUvFragpos.spv", {FRAG_UV, FRAG_POSITION}},
                 {"shaders/PosUvNormToUvFragposFragnorm.spv", {FRAG_POSITION, FRAG_UV, FRAG_NORMAL}}

         };

         _vertexDescriptors.insert({VERTEX_WITH_NORMAL, {vd, posUvNormShaders}});


         vd.init(
                 _bp,
                 {
                         {0, sizeof(VertexWithNormalAndTan), VK_VERTEX_INPUT_RATE_VERTEX}
                 },
                 {
                         {0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VertexWithNormalAndTan, pos),sizeof(glm::vec3), POSITION},
                         {0, 1, VK_FORMAT_R32G32_SFLOAT,    offsetof(VertexWithNormalAndTan, UV),sizeof(glm::vec2), UV},
                         {0, 2, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VertexWithNormalAndTan, norm),sizeof(glm::vec3), NORMAL},
                         {0, 3, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(VertexWithNormalAndTan, tan),sizeof(glm::vec4), TANGENT}
                 }
         );

         std::set<VertexShader> posUvNormTanShaders = {
                 {"shaders/PosUvNormTanToUv.spv", {FRAG_UV}},
                 {"shaders/PosUvNormTanToUvFragpos.spv", {FRAG_UV, FRAG_POSITION}},
                 {"shaders/PosUvNormTanToUvFragposFragnorm.spv", {FRAG_POSITION, FRAG_UV, FRAG_NORMAL}},
                 {"shaders/PosUvNormTanToUvFragposFragnormFragtan.spv", {FRAG_POSITION, FRAG_UV, FRAG_NORMAL, FRAG_TANGENT}}
         };

         _vertexDescriptors.insert({VERTEX_WITH_NORMAL_AND_TANGENT, {vd, posUvNormTanShaders}});


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
            case VERTEX_WITH_NORMAL_AND_TANGENT:
                model = std::make_unique<Model<VertexWithNormalAndTan>>();
                break;
            default:
                throw std::runtime_error("Remember to add new vertex layout in model loading code");
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

    std::unordered_map<VertexType, std::pair<VertexDescriptor, std::set<VertexShader>>>& ModelSystem::getAllVertexDescriptors(){
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