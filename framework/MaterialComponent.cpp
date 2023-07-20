//
// Created by drgio on 20/07/2023.
//

#include "MaterialComponent.h"

#include <utility>

namespace fmwk {
    MaterialComponent::MaterialComponent(const std::string &name, EffectType effectType): Component(name), _effectType(effectType), _alreadyProvisioned(false), _pipeline(nullptr){}

    void MaterialComponent::provision(DescriptorSet descriptorSet, Pipeline* pipeline) {
        _descriptorSet = std::move(descriptorSet);
        _pipeline = pipeline;
    }

    bool MaterialComponent::isProvisioned() const {
        return _alreadyProvisioned;
    }

    EffectType MaterialComponent::getEffectType() {
        return _effectType;
    }

} // fmwk