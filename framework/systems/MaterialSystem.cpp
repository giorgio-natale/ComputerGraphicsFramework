//
// Created by drgio on 19/07/2023.
//

#include "MaterialSystem.h"

namespace fmwk {
    MaterialSystem::MaterialSystem(BaseProject *bp):_bp(bp){}

    void MaterialSystem::bootSystem() {
        DescriptorSetLayout defaultLayout{};

        //default effect
        defaultLayout.init(_bp, {{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT}});
        Effect defaultEffect;
        defaultEffect.type = DEFAULT;
        defaultEffect.layout = defaultLayout;
        defaultEffect.shaderName = "shaders/Default.spv";
        defaultEffect.requiredFeatures = {FRAG_UV};
        _effects.insert({defaultEffect.type, defaultEffect});

        DescriptorSetLayout colorBlendLayout{};

        //color blend effect
        colorBlendLayout.init(_bp, {{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT}});
        Effect colorBlendEffect;
        colorBlendEffect.type = COLOR_BLEND;
        colorBlendEffect.layout = colorBlendLayout;
        colorBlendEffect.shaderName = "shaders/ColorBlend.spv";
        colorBlendEffect.requiredFeatures = {FRAG_UV};
        _effects.insert({COLOR_BLEND, colorBlendEffect});

        //simple phong effect
        DescriptorSetLayout simplePhongLayout{};
        simplePhongLayout.init(_bp, {{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT}});
        Effect simplePhongEffect;
        simplePhongEffect.type = SIMPLE_PHONG;
        simplePhongEffect.layout = simplePhongLayout;
        simplePhongEffect.shaderName = "shaders/SimplePhong.spv";
        simplePhongEffect.requiredFeatures = {FRAG_POSITION, FRAG_UV, FRAG_NORMAL};

        _effects.insert({SIMPLE_PHONG, simplePhongEffect});

        //ggx effect
        DescriptorSetLayout ggxLayout{};
        ggxLayout.init(_bp, {
            {0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT},
            {1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT},
            {2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT}
        });

        Effect ggxEffect;
        ggxEffect.type = GGX;
        ggxEffect.layout = ggxLayout;
        ggxEffect.shaderName = "shaders/GGXTest.spv";
        ggxEffect.requiredFeatures = {FRAG_POSITION, FRAG_UV, FRAG_NORMAL, FRAG_TANGENT};


        _effects.insert({GGX, ggxEffect});


    }

    Effect &MaterialSystem::getEffectByType(EffectType type) {
        if(_effects.find(type) == _effects.end())
            throw std::runtime_error("Cannot find an effect from its type. Check that all effects are correctly added to the effect map");
        return _effects.find(type)->second;
    }

    std::unordered_map<EffectType, Effect> &MaterialSystem::getAllEffects() {
        return _effects;
    }
} // fmwk