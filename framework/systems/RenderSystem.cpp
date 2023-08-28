//
// Created by drgio on 20/07/2023.
//

#include "RenderSystem.h"
#include "../Entity.h"
#include "../components/lights/LightComponent.h"
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtx/string_cast.hpp>

namespace fmwk {
    RenderSystem::RenderSystem(BaseProject *bp):_bp(bp){}

    void RenderSystem::bootSystem(
            DescriptorSetLayout& textureDescriptorSetLayout,
            std::unordered_map<VertexType, std::pair<VertexDescriptor, std::set<VertexShader>>>& _vertexDescriptors,
            std::unordered_map<EffectType, Effect> &_effects) {

        _globalDescriptorSetLayout.init(_bp, {
            {0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT},
            {1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT}
        });
        _modelDescriptorSetLayout.init(_bp, {{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT}});
        _textureDescriptorSetLayout = textureDescriptorSetLayout;

        _globalOverlayDescriptorSetLayout.init(_bp, {{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT}});
        _modelOverlayDescriptorSetLayout.init(_bp, {{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT}});

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

                //if(bestVertexShader == nullptr)
                //    throw std::runtime_error("Cannot find an appropriate vertex shader for an effect (fragment shader: " + effectVal.shaderName + ")");

                if(bestVertexShader != nullptr) {
                    auto [insertedPipelineIterator, ok] = _pipelines.insert({{vertexKey, effectKey},
                                                                             {}});
                    if(vertexKey != VERTEX_OVERLAY) {
                        insertedPipelineIterator->second.init(_bp,
                                                              &vertexVal.first,
                                                              bestVertexShader->fileName,
                                                              effectVal.shaderName,
                                                              {&_globalDescriptorSetLayout,
                                                               &_textureDescriptorSetLayout,
                                                               &effectVal.layout, &_modelDescriptorSetLayout});
                        if (effectVal.isTransparent) {
                            insertedPipelineIterator->second.setAdvancedFeatures(VK_COMPARE_OP_LESS,
                                                                                 VK_POLYGON_MODE_FILL,
                                                                                 VK_CULL_MODE_BACK_BIT, true);
                        }
                    }else{
                        insertedPipelineIterator->second.init(_bp,
                                                              &vertexVal.first,
                                                              bestVertexShader->fileName,
                                                              effectVal.shaderName,
                                                              {
                                                                      &_globalOverlayDescriptorSetLayout,
                                                                      &_textureDescriptorSetLayout,
                                                                      &effectVal.layout,
                                                                      &_modelOverlayDescriptorSetLayout
                                                              });
                        insertedPipelineIterator->second.setAdvancedFeatures(VK_COMPARE_OP_LESS_OR_EQUAL,
                                                                             VK_POLYGON_MODE_FILL,
                                                                             VK_CULL_MODE_NONE, true);
                    }

                    std::cout << "[PIPELINE CREATED]: Vertex Shader: " + bestVertexShader->fileName +
                                 ", Fragment Shader: " + effectVal.shaderName << std::endl;
                }
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

    DescriptorSet &RenderSystem::getGlobalOverlayDescriptorSet() {
        return _globalOverlayDescriptorSet;
    }

    DescriptorSetLayout &RenderSystem::getModelOverlayDescriptorSetLayout() {
        return _modelOverlayDescriptorSetLayout;
    }

    void RenderSystem::updateGlobalDescriptor(Camera* cameraComponent, std::vector<LightComponent*>& lights, float aspectRatio, float virtualScreenWidthUnits, int currentImage) {
        GlobalUniformBlock gubo{};
        gubo.vpMat = cameraComponent->getProjectionMatrix() * cameraComponent->getViewMatrix();

        GlobalOverlayUniformBlock goubo{};

        auto scaling = glm::scale(glm::mat3(1), 2.0f * glm::vec2(1/virtualScreenWidthUnits, 1/ ((virtualScreenWidthUnits) / aspectRatio)));
        auto changeBase = glm::translate(glm::mat3(1), glm::vec2(-1,1)) * glm::scale(glm::mat3(1), glm::vec2(1, -1));

        goubo.overlayProjectionMatrix = changeBase * scaling;

        //std::cout << glm::to_string(glm::mat3(goubo.overlayProjectionMatrix) * glm::vec3(0,0,1)) << std::endl;
        /*goubo.overlayProjectionMatrix = glm::mat4(glm::mat3(
                {1, 0, 0},
                {0, 1, 0},
                {0,0,1}
        ))*/;


        GlobalLightUniformBlock glubo{};
        glubo.eyePosition = cameraComponent->getParent()->getTransform().getPosition();

        DirectLightBlock directionalLightBlock{};
        glubo.directLightsCount = 0;
        glubo.pointLightsCount = 0;
        glubo.spotLightsCount = 0;

        for(LightComponent* light : lights){
            if(light->isTurnedOn())
                light->updateLightUniformBlock(glubo);
        }

        _globalDescriptorSet.map(currentImage, &gubo, sizeof(gubo), 0);
        _globalDescriptorSet.map(currentImage, &glubo, sizeof(glubo), 1);

        _globalOverlayDescriptorSet.map(currentImage, &goubo, sizeof(goubo), 0);

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
        _globalDescriptorSet.init(_bp, &_globalDescriptorSetLayout, {
            {0, UNIFORM, sizeof(GlobalUniformBlock)},
            {1, UNIFORM, sizeof(GlobalLightUniformBlock)}
        });
        _globalOverlayDescriptorSet.init(_bp, &_globalOverlayDescriptorSetLayout, {
                {0, UNIFORM, sizeof(GlobalOverlayUniformBlock)}
        });
    }

    void RenderSystem::cleanupGlobalDescriptorSet() {
        _globalDescriptorSet.cleanup();
        _globalOverlayDescriptorSet.cleanup();
    }

    void RenderSystem::destroyDescriptorSetLayouts() {
        _globalDescriptorSetLayout.cleanup();
        _textureDescriptorSetLayout.cleanup();
        _modelDescriptorSetLayout.cleanup();

        _globalOverlayDescriptorSetLayout.cleanup();
        _modelOverlayDescriptorSetLayout.cleanup();
    }

} // fmwk