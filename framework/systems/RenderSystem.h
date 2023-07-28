//
// Created by drgio on 20/07/2023.
//

#ifndef DEMO_RENDERSYSTEM_H
#define DEMO_RENDERSYSTEM_H

#include "../../Starter.hpp"
#include "ModelSystem.h"
#include "MaterialSystem.h"
#include "../Camera.h"

#define DIRECT_LIGHTS_MAX 10
#define POINT_LIGHTS_MAX 10
#define SPOT_LIGHTS_MAX 10

namespace std{
    template <>
    struct hash<std::pair<fmwk::VertexType, fmwk::EffectType>>
    {
        std::size_t operator()(const std::pair<fmwk::VertexType, fmwk::EffectType>& k) const
        {
            return k.second * 100 + k.first;
        }
    };
}

namespace fmwk {

    struct DirectionalLightBlock{
        alignas(16) glm::vec3 lightDir;
        alignas(16) glm::vec4 lightColor;
    };

    struct PointLightBlock{
        alignas(16) glm::vec3 lightPos;
        alignas(16) glm::vec4 lightColor;
        alignas(4) float beta;
        alignas(4) float g;
    };

    struct SpotLightBlock{
        alignas(16) glm::vec3 lightPos;
        alignas(16) glm::vec3 lightDir;
        alignas(16) glm::vec4 lightColor;
        alignas(4) float beta;
        alignas(4) float g;
        alignas(4) float cosOuter;
        alignas(4) float cosInner;
    };

    struct GlobalUniformBlock{
        alignas(16) glm::mat4 vpMat;
    };
    struct GlobalLightUniformBlock{
        alignas(16) glm::vec3 eyePosition;
        alignas(16) DirectionalLightBlock directLights[DIRECT_LIGHTS_MAX];
        alignas(16) PointLightBlock pointLights[POINT_LIGHTS_MAX];
        alignas(16) SpotLightBlock spotLights[SPOT_LIGHTS_MAX];
        alignas(4) int directLightsCount;
        alignas(4) int pointLightsCount;
        alignas(4) int spotLightsCount;
    };

    class RenderSystem {
    private:
        BaseProject *_bp;
        DescriptorSetLayout _globalDescriptorSetLayout{};
        DescriptorSetLayout _textureDescriptorSetLayout{};
        DescriptorSetLayout _modelDescriptorSetLayout{};
        DescriptorSet _globalDescriptorSet;
        std::unordered_map<std::pair<VertexType, EffectType>, Pipeline> _pipelines;

    public:
        explicit RenderSystem(BaseProject *bp);
        void bootSystem(
                DescriptorSetLayout& textureDescriptorSetLayout,
                std::unordered_map<VertexType, std::pair<VertexDescriptor, std::set<VertexShader>>>& _vertexDescriptors,
                std::unordered_map<EffectType, Effect>& _effects
                );
        Pipeline& getPipeline(VertexType vertexType, EffectType effectType);
        DescriptorSet& getGlobalDescriptorSet();
        DescriptorSetLayout& getModelDescriptorSetLayout();
        void updateGlobalDescriptor(Camera* cameraComponent, int currentImage);

        void rebuildPipelines();
        void resetPipelines();
        void destroyPipelines();

        void rebuildGlobalDescriptorSet();
        void cleanupGlobalDescriptorSet();
    };

} // fmwk

#endif //DEMO_RENDERSYSTEM_H
