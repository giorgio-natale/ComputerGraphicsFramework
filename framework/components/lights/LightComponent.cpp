//
// Created by drgio on 28/07/2023.
//

#include "LightComponent.h"

namespace fmwk {
    LightComponent::LightComponent(const std::string &name) : Component(name) {
        _alreadyProvisioned = false;
        _isTurnedOn = true;
        _descriptorSet = nullptr;
    }

    bool LightComponent::isProvisioned() const {
        return _alreadyProvisioned;
    }

    void LightComponent::provision(DescriptorSet *descriptorSet) {
        _descriptorSet = descriptorSet;
        _alreadyProvisioned = true;
    }

    void LightComponent::turnOn() {
        _isTurnedOn = true;
    }

    void LightComponent::turnOff() {
        _isTurnedOn = false;
    }

    bool LightComponent::isTurnedOn() const {
        return _isTurnedOn;
    }

} // fmwk