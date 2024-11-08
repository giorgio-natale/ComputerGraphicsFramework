//
// Created by drgio on 20/07/2023.
//

#include "MaterialComponent.h"

#include <utility>

namespace fmwk {
    MaterialComponent::MaterialComponent(EffectType effectType): Component("Material"), _effectType(effectType), _alreadyProvisioned(false), _pipeline(nullptr), _descriptorSet(nullptr){}

    void MaterialComponent::provision(DescriptorSet* descriptorSet, Pipeline* pipeline) {
        _descriptorSet = descriptorSet;
        _pipeline = pipeline;
        _alreadyProvisioned = true;
    }

    bool MaterialComponent::isProvisioned() const {
        return _alreadyProvisioned;
    }

    EffectType MaterialComponent::getEffectType() {
        return _effectType;
    }

    Pipeline *MaterialComponent::getPipeline() const {
        return _pipeline;
    }

    DescriptorSet &MaterialComponent::getDescriptorSet() {
        return *_descriptorSet;
    }

} // fmwk