//
// Created by drgio on 20/07/2023.
//

#include "DefaultMaterial.h"

namespace fmwk {
    void DefaultMaterial::updateDescriptorSet(int currentImage) {
        DefaultMaterialUniformBlock ubo{};
        ubo.brightness = _brightness;
        _descriptorSet->map(currentImage, &ubo, sizeof(ubo), 0);
    }

    void DefaultMaterial::update() {
        _brightness -= 0.001f;
        std::cout << "BRIGHTNESS UPDATED: " << _brightness << std::endl;
    }

    DescriptorSetClaim DefaultMaterial::getDescriptorSetClaim() {
        return {{0, UNIFORM, sizeof(DefaultMaterialUniformBlock)}};
    }

    DefaultMaterial::DefaultMaterial(const std::string &name,float brightness) : MaterialComponent(name, DEFAULT), _brightness(brightness) {}
} // fmwk