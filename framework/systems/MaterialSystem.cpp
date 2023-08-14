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
        defaultEffect.shaderName = "shaders/compiled/Default.spv";
        defaultEffect.requiredFeatures = {FRAG_UV};
        defaultEffect.isTransparent = false;
        _effects.insert({defaultEffect.type, defaultEffect});

        DescriptorSetLayout colorBlendLayout{};

        //color blend effect
        colorBlendLayout.init(_bp, {{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT}});
        Effect colorBlendEffect;
        colorBlendEffect.type = COLOR_BLEND;
        colorBlendEffect.layout = colorBlendLayout;
        colorBlendEffect.shaderName = "shaders/compiled/ColorBlend.spv";
        colorBlendEffect.requiredFeatures = {FRAG_UV};
        colorBlendEffect.isTransparent = false;
        _effects.insert({COLOR_BLEND, colorBlendEffect});

        //simple phong effect
        DescriptorSetLayout simplePhongLayout{};
        simplePhongLayout.init(_bp, {{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT}});
        Effect simplePhongEffect;
        simplePhongEffect.type = SIMPLE_PHONG;
        simplePhongEffect.layout = simplePhongLayout;
        simplePhongEffect.shaderName = "shaders/compiled/SimplePhong.spv";
        simplePhongEffect.requiredFeatures = {FRAG_POSITION, FRAG_UV, FRAG_NORMAL};
        simplePhongEffect.isTransparent = false;
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
        ggxEffect.shaderName = "shaders/compiled/GGXTest.spv";
        ggxEffect.requiredFeatures = {FRAG_POSITION, FRAG_UV, FRAG_NORMAL, FRAG_TANGENT};
        ggxEffect.isTransparent = false;

        _effects.insert({GGX, ggxEffect});


        DescriptorSetLayout blinkLayout{};

        //blink effect
        blinkLayout.init(_bp, {{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT}});
        Effect blinkEffect;
        blinkEffect.type = SIMPLE_PHONG_BLINK;
        blinkEffect.layout = blinkLayout;
        blinkEffect.shaderName = "shaders/compiled/SimplePhongBlink.spv";
        blinkEffect.requiredFeatures = {FRAG_POSITION, FRAG_UV, FRAG_NORMAL};
        blinkEffect.isTransparent = true;
        _effects.insert({blinkEffect.type, blinkEffect});


        DescriptorSetLayout defaultTransparentLayout{};

        //default transparent effect
        defaultTransparentLayout.init(_bp, {{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT}});
        Effect defaultTransparentEffect;
        defaultTransparentEffect.type = DEFAULT_TRANSPARENT;
        defaultTransparentEffect.layout = defaultTransparentLayout;
        defaultTransparentEffect.shaderName = "shaders/compiled/DefaultTransparent.spv";
        defaultTransparentEffect.requiredFeatures = {FRAG_UV};
        defaultTransparentEffect.isTransparent = true;
        _effects.insert({defaultTransparentEffect.type, defaultTransparentEffect});

        DescriptorSetLayout phongBlendColorLayout{};

        //simple phong color blend
        phongBlendColorLayout.init(_bp, {{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT}});
        Effect phongBlendColorEffect;
        phongBlendColorEffect.type = SIMPLE_PHONG_COLOR_BLEND;
        phongBlendColorEffect.layout = phongBlendColorLayout;
        phongBlendColorEffect.shaderName = "shaders/compiled/SimplePhongColorBlend.spv";
        phongBlendColorEffect.requiredFeatures = {FRAG_POSITION, FRAG_UV, FRAG_NORMAL};
        phongBlendColorEffect.isTransparent = false;
        _effects.insert({phongBlendColorEffect.type, phongBlendColorEffect});

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