//
// Created by drgio on 20/07/2023.
//

#include "RenderSystem.h"

namespace fmwk {
    RenderSystem::RenderSystem(BaseProject *bp):_bp(bp){}

    void RenderSystem::bootSystem(
            DescriptorSetLayout& textureDescriptorSetLayout,
            std::unordered_map<VertexType, std::pair<VertexDescriptor, std::set<VertexShader>>>& _vertexDescriptors,
            std::unordered_map<EffectType, Effect> &_effects) {

        _globalDescriptorSetLayout.init(_bp, {{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT}});
        _modelDescriptorSetLayout.init(_bp, {{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT}});
        _textureDescriptorSetLayout = textureDescriptorSetLayout;

        for (auto& [vertexKey, vertexVal]: _vertexDescriptors) {
            for(auto& [effectKey, effectVal]: _effects){
                auto requiredFeatures = effectVal.requiredFeatures;
                auto& possibleShaders = vertexVal.second;

                size_t bestShaderScore = INT_MAX;
                VertexShader const* bestVertexShader = nullptr;
                for(auto& vertexShader : possibleShaders){
                    std::set<VertexShaderOutputFeature> testSet;
                    std::set_difference(requiredFeatures.begin(), requiredFeatures.end(), vertexShader.outputFeatures.begin(), vertexShader.outputFeatures.end(), std::inserter(testSet, testSet.end()));
                    if(testSet.empty()){
                        testSet.clear();
                        std::set_difference(vertexShader.outputFeatures.begin(), vertexShader.outputFeatures.end(), requiredFeatures.begin(), requiredFeatures.end(), std::inserter(testSet, testSet.end()));
                        if(testSet.size() < bestShaderScore){
                            bestShaderScore = testSet.size();
                            bestVertexShader = &vertexShader;
                            if(bestShaderScore == 0)
                                break;
                        }
                    }
                }

                if(bestVertexShader == nullptr)
                    throw std::runtime_error("Cannot find an appropriate vertex shader for an effect (fragment shader: " + effectVal.shaderName + ")");

                auto [insertedPipelineIterator, ok] = _pipelines.insert({{vertexKey, effectKey}, {}});
                insertedPipelineIterator->second.init(_bp,
                                                      &vertexVal.first,
                                                      bestVertexShader->fileName,
                                                      effectVal.shaderName,
                                                      {&_globalDescriptorSetLayout, &_textureDescriptorSetLayout, &effectVal.layout, &_modelDescriptorSetLayout});

                std::cout << "[PIPELINE CREATED]: Vertex Shader: " + bestVertexShader->fileName + ", Fragment Shader: " + effectVal.shaderName << std::endl;
            }
        }

    }

    Pipeline &RenderSystem::getPipeline(VertexType vertexType, EffectType effectType) {
        return _pipelines.find({vertexType, effectType})->second;
    }

    DescriptorSet &RenderSystem::getGlobalDescriptorSet() {
        return _globalDescriptorSet;
    }

    DescriptorSetLayout &RenderSystem::getModelDescriptorSetLayout() {
        return _modelDescriptorSetLayout;
    }

    void RenderSystem::updateGlobalDescriptor(Camera* cameraComponent, int currentImage) {
        GlobalUniformBlock gubo{};
        gubo.vpMat = cameraComponent->getProjectionMatrix() * cameraComponent->getViewMatrix();
        _globalDescriptorSet.map(currentImage, &gubo, sizeof(gubo), 0);
    }

    void RenderSystem::rebuildPipelines() {
        for(auto& [key, pipeline] : _pipelines)
            pipeline.create();
    }

    void RenderSystem::resetPipelines() {
        for(auto& [key, pipeline] : _pipelines)
            pipeline.cleanup();
    }

    void RenderSystem::destroyPipelines() {
        for(auto& [key, pipeline] : _pipelines)
            pipeline.destroy();
    }

    void RenderSystem::rebuildGlobalDescriptorSet() {
        _globalDescriptorSet.init(_bp, &_globalDescriptorSetLayout, {{0, UNIFORM, sizeof(GlobalUniformBlock)}});
    }

    void RenderSystem::cleanupGlobalDescriptorSet() {
        _globalDescriptorSet.cleanup();
    }

} // fmwk