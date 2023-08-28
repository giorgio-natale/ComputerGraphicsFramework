//
// Created by drgio on 18/07/2023.
//

#ifndef DEMO_MODELSYSTEM_H
#define DEMO_MODELSYSTEM_H

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <memory>
#include "../../Starter.hpp"
#include "../components/transform/Transform.h"

namespace fmwk {

    struct Vertex {
        glm::vec3 pos;
        glm::vec2 UV;
    };
    struct VertexWithNormal{
        glm::vec3 pos;
        glm::vec2 UV;
        glm::vec3 norm;
    };

    struct VertexWithNormalAndTan{
        glm::vec3 pos;
        glm::vec2 UV;
        glm::vec3 norm;
        glm::vec4 tan;
    };

    struct VertexOverlay{
        glm::vec2 pos;
        glm::vec2 UV;
    };
    enum VertexType{VERTEX, VERTEX_WITH_NORMAL, VERTEX_WITH_NORMAL_AND_TANGENT, VERTEX_OVERLAY};

    enum VertexShaderOutputFeature{FRAG_UV, FRAG_POSITION, FRAG_NORMAL, FRAG_TANGENT};

    struct VertexShader{
        std::string fileName;
        std::set<VertexShaderOutputFeature> outputFeatures;
    };

    inline bool operator<(const VertexShader& lhs, const VertexShader& rhs)
    {
        return lhs.fileName < rhs.fileName;
    }


    class TModel{
    private:
        std::unique_ptr<BaseModel> _model;
        VertexType _type;
        std::unique_ptr<Transform> _modelTransform;
    public:
        TModel(std::unique_ptr<BaseModel> model, VertexType type, glm::vec3 position, glm::quat quaternion, glm::vec3 scale);
        VertexType getType();
        BaseModel& getTypedModel();
        Transform& getModelTransform();
        void destroyModel();
    };

    class ModelSystem {
    private:
        BaseProject *_bp;
        std::unordered_map<VertexType, std::pair<VertexDescriptor, std::set<VertexShader>>> _vertexDescriptors;
        std::unordered_map<std::string, TModel> _models;

        static std::unique_ptr<BaseModel> createEmptyModel(VertexType vertexType);

    public:
        explicit ModelSystem(BaseProject *bp);

        void addModel(std::string const& name, VertexType vertexType, std::string const& fileName);
        void addModel(std::string const& name, VertexType vertexType, std::string const& fileName, glm::vec3 position, glm::quat quaternion, glm::vec3 scale);

        template<class Vert>
        void addModel(const std::string &name, VertexType vertexType, std::vector<Vert> const& vertices,
                                   std::vector<uint32_t> const& indices) {

            addModel(name, vertexType, vertices, indices, glm::vec3(0,0,0), createQuat(Y, 180), glm::vec3(1));
        }

        template<class Vert>
        void addModel(const std::string &name, VertexType vertexType, std::vector<Vert> const& vertices,
                      std::vector<uint32_t> const& indices, glm::vec3 position, glm::quat quaternion, glm::vec3 scale) {

            std::unique_ptr<Model<Vert>> rawModel = std::make_unique<Model<Vert>>();
            rawModel->vertices = vertices;
            rawModel->indices = indices;
            rawModel->initMesh(_bp, &_vertexDescriptors.find(vertexType)->second.first);

            std::unique_ptr<BaseModel> model = std::move(rawModel);
            _models.insert({name, TModel(std::move(model), vertexType, position, quaternion, scale)});
        }


        TModel& getModelByName(std::string const& name);

        std::unordered_map<VertexType, std::pair<VertexDescriptor, std::set<VertexShader>>>& getAllVertexDescriptors();

        void destroyModels();

    };

} // fmwk

#endif //DEMO_MODELSYSTEM_H
