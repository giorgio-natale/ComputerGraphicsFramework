//
// Created by drgio on 18/07/2023.
//

#ifndef DEMO_MODELSYSTEM_H
#define DEMO_MODELSYSTEM_H

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <memory>
#include "../../Starter.hpp"

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
    enum VertexType{VERTEX, VERTEX_WITH_NORMAL};

    class TModel{
    private:
        std::unique_ptr<BaseModel> _model;
        VertexType _type;
    public:
        TModel(std::unique_ptr<BaseModel> model, VertexType type);
        VertexType getType();
        BaseModel& getTypedModel();
    };

    class ModelSystem {
    private:
        BaseProject *_bp;
        std::unordered_map<VertexType, std::pair<VertexDescriptor, std::string>> _vertexDescriptors;
        std::unordered_map<std::string, TModel> _models;

    public:
        explicit ModelSystem(BaseProject *bp);

        void addModel(std::string const& name, VertexType vertexType, std::string const& fileName);

        TModel& getModelByName(std::string const& name);

        std::unordered_map<VertexType, std::pair<VertexDescriptor, std::string>>& getAllVertexDescriptors();

    };

} // fmwk

#endif //DEMO_MODELSYSTEM_H