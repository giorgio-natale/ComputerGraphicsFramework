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

        //default effect
        colorBlendLayout.init(_bp, {{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT}});
        Effect colorBlendEffect;
        colorBlendEffect.type = COLOR_BLEND;
        colorBlendEffect.layout = colorBlendLayout;
        colorBlendEffect.shaderName = "shaders/ColorBlend.spv";
        colorBlendEffect.requiredFeatures = {FRAG_UV};
        _effects.insert({COLOR_BLEND, colorBlendEffect});
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