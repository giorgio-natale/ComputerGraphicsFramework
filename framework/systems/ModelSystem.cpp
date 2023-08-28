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
                {"shaders/compiled/PosUvToUv.spv", {FRAG_UV}},
                {"shaders/compiled/PosUvToUvFragPos.spv", {FRAG_UV, FRAG_POSITION}}
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
                 {"shaders/compiled/PosUvNormToUv.spv", {FRAG_UV}},
                 {"shaders/compiled/PosUvNormToUvFragpos.spv", {FRAG_UV, FRAG_POSITION}},
                 {"shaders/compiled/PosUvNormToUvFragposFragnorm.spv", {FRAG_POSITION, FRAG_UV, FRAG_NORMAL}}

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
                 {"shaders/compiled/PosUvNormTanToUv.spv", {FRAG_UV}},
                 {"shaders/compiled/PosUvNormTanToUvFragpos.spv", {FRAG_UV, FRAG_POSITION}},
                 {"shaders/compiled/PosUvNormTanToUvFragposFragnorm.spv", {FRAG_POSITION, FRAG_UV, FRAG_NORMAL}},
                 {"shaders/compiled/PosUvNormTanToUvFragposFragnormFragtan.spv", {FRAG_POSITION, FRAG_UV, FRAG_NORMAL, FRAG_TANGENT}}
         };

         _vertexDescriptors.insert({VERTEX_WITH_NORMAL_AND_TANGENT, {vd, posUvNormTanShaders}});


         vd.init(_bp,
                 {
                    {0, sizeof(VertexOverlay), VK_VERTEX_INPUT_RATE_VERTEX}
                 },
                 {
                         {0, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(VertexOverlay, pos),
                                 sizeof(glm::vec2), OTHER},
                         {0, 1, VK_FORMAT_R32G32_SFLOAT, offsetof(VertexOverlay, UV),
                                 sizeof(glm::vec2), UV}
                 });

         std::set<VertexShader> overlayShaders = {
                 {"shaders/compiled/OverlayVert.spv", {FRAG_UV}},
         };

         _vertexDescriptors.insert({VERTEX_OVERLAY, {vd, overlayShaders}});
    }

    void ModelSystem::addModel(std::string const& name, VertexType vertexType, std::string const& fileName){
         addModel(name, vertexType, fileName, glm::vec3(0,0,0), createQuat(Y, 180), glm::vec3(1));
    }

    void ModelSystem::addModel(const std::string &name, VertexType vertexType, const std::string &fileName,
                               glm::vec3 position, glm::quat quaternion, glm::vec3 scale) {
        if(_models.find(name) != _models.end())
            throw std::runtime_error("Could not add model with name '" + name + "' because there was another model with the same name");
        auto extension = fileName.substr(fileName.find_last_of('.') + 1);
        ModelType modelType;
        if(extension == "gltf" || extension == "glb") {
            modelType = GLTF;
        } else if(extension == "obj") {
            modelType = OBJ;
        } else if(extension == "mgcg"){
            modelType = MGCG;
        } else {
            throw std::runtime_error("Cannot recognize model extension '" + extension + "'");
        }

        std::unique_ptr<BaseModel> model = createEmptyModel(vertexType);
        model->init(_bp, &_vertexDescriptors.find(vertexType)->second.first, fileName, modelType);
        _models.insert({name, TModel(std::move(model), vertexType, position, quaternion, scale)});
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

    std::unique_ptr<BaseModel> ModelSystem::createEmptyModel(VertexType vertexType) {
        switch (vertexType) {
            case VERTEX: {
                return std::make_unique<Model<Vertex>>();
            }
            case VERTEX_WITH_NORMAL:
                return std::make_unique<Model<VertexWithNormal>>();
            case VERTEX_WITH_NORMAL_AND_TANGENT:
                return std::make_unique<Model<VertexWithNormalAndTan>>();
            default:
                throw std::runtime_error("Remember to add new vertex layout in model loading code");
        }
    }


    TModel::TModel(std::unique_ptr<BaseModel> model, VertexType type, glm::vec3 position, glm::quat quaternion, glm::vec3 scale){
         _model = std::move(model);
         _type = type;
         _modelTransform = std::make_unique<Transform>(position, quaternion, scale);
     }

    VertexType TModel::getType() {
        return _type;
    }

    BaseModel &TModel::getTypedModel() {
        return *_model;
    }

    Transform &TModel::getModelTransform() {
        return *_modelTransform;
    }

    void TModel::destroyModel() {
        _model->cleanup();
    }

    void ModelSystem::destroyModels() {
        for(auto& [name, model] : _models){
            model.destroyModel();
        }
    }
} // fmwk