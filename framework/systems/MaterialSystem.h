//
// Created by drgio on 19/07/2023.
//

#ifndef DEMO_MATERIALSYSTEM_H
#define DEMO_MATERIALSYSTEM_H

#include "../../Starter.hpp"
#include "ModelSystem.h"

namespace fmwk {
    enum EffectType{DEFAULT, COLOR_BLEND};

    struct Effect{
        DescriptorSetLayout layout;
        EffectType type;
        std::string shaderName;
        std::set<VertexShaderOutputFeature> requiredFeatures;
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
