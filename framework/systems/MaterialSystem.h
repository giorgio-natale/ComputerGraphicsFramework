//
// Created by drgio on 19/07/2023.
//

#ifndef DEMO_MATERIALSYSTEM_H
#define DEMO_MATERIALSYSTEM_H

#include "../../Starter.hpp"
#include "ModelSystem.h"

namespace fmwk {
    enum EffectType{DEFAULT, COLOR_BLEND, SIMPLE_PHONG, GGX, SIMPLE_PHONG_BLINK, DEFAULT_TRANSPARENT, SIMPLE_PHONG_COLOR_BLEND, DEFAULT_SPRITE};

    struct Effect{
        DescriptorSetLayout layout;
        EffectType type;
        std::string shaderName;
        std::set<VertexShaderOutputFeature> requiredFeatures;
        bool isTransparent;
    };

    class MaterialSystem {
    private:
        std::unordered_map<EffectType, Effect> _effects;
        BaseProject *_bp;

    public:
        explicit MaterialSystem(BaseProject *bp);
        void bootSystem();
        Effect& getEffectByType(EffectType type);
        std::unordered_map<EffectType, Effect>& getAllEffects();
    };

} // fmwk

#endif //DEMO_MATERIALSYSTEM_H
