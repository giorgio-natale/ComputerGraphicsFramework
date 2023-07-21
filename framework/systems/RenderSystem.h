//
// Created by drgio on 20/07/2023.
//

#ifndef DEMO_RENDERSYSTEM_H
#define DEMO_RENDERSYSTEM_H

#include "../../Starter.hpp"
#include "ModelSystem.h"
#include "MaterialSystem.h"
#include "../Camera.h"

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

    struct GlobalUniformBlock{
        alignas(16) glm::mat4 vpMat;
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
                std::unordered_map<VertexType, std::pair<VertexDescriptor, std::string>>& _vertexDescriptors,
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
