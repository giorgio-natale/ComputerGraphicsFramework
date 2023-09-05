//
// Created by drgio on 20/07/2023.
//

#ifndef DEMO_MATERIALCOMPONENT_H
#define DEMO_MATERIALCOMPONENT_H

#include "../scripts/Component.h"
#include "../../../Starter.hpp"
#include "../../systems/MaterialSystem.h"

namespace fmwk {

    typedef std::vector<DescriptorSetElement> DescriptorSetClaim;

    class MaterialComponent : public Component{
    private:
        Pipeline* _pipeline;
        EffectType _effectType;
        bool _alreadyProvisioned;
    public:
        explicit MaterialComponent(EffectType effectType);
        virtual void updateDescriptorSet(int currentImage) = 0;
        virtual DescriptorSetClaim getDescriptorSetClaim() = 0;
        void provision(DescriptorSet *descriptorSet, Pipeline* pipeline);
        DescriptorSet& getDescriptorSet();
        [[nodiscard]] bool isProvisioned() const override;
        EffectType getEffectType();
        [[nodiscard]] Pipeline* getPipeline() const;
    protected:
        DescriptorSet* _descriptorSet;
    };

} // fmwk

#endif //DEMO_MATERIALCOMPONENT_H
