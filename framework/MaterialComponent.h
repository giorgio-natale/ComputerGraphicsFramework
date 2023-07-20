//
// Created by drgio on 20/07/2023.
//

#ifndef DEMO_MATERIALCOMPONENT_H
#define DEMO_MATERIALCOMPONENT_H

#include "Component.h"
#include "../Starter.hpp"
#include "systems/MaterialSystem.h"

namespace fmwk {

    typedef std::vector<DescriptorSetElement> DescriptorSetClaim;

    class MaterialComponent : public Component{
    private:
        DescriptorSet _descriptorSet;
        Pipeline* _pipeline;
        EffectType _effectType;
        bool _alreadyProvisioned;
    public:
        MaterialComponent(std::string const& name, EffectType effectType);
        virtual void updateDescriptorSet(int currentImage) = 0;
        virtual DescriptorSetClaim getDescriptorSetClaim() = 0;
        void provision(DescriptorSet descriptorSet, Pipeline* pipeline);
        [[nodiscard]] bool isProvisioned() const override;
        EffectType getEffectType();
    };

} // fmwk

#endif //DEMO_MATERIALCOMPONENT_H
