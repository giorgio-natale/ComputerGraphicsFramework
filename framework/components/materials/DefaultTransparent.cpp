//
// Created by drgio on 14/08/2023.
//

#include "DefaultTransparent.h"

namespace fmwk {
    DefaultTransparent::DefaultTransparent(float brightness, float transparency) : MaterialComponent(DEFAULT_TRANSPARENT),
    _brightness(brightness),
    _transparency(transparency){}

    void DefaultTransparent::updateDescriptorSet(int currentImage) {
        DefaultTransparentMaterialUniformBlock ubo{};
        ubo.brightness = _brightness;
        ubo.transparency = _transparency;
        _descriptorSet->map(currentImage, &ubo, sizeof(ubo), 0);
    }

    DescriptorSetClaim DefaultTransparent::getDescriptorSetClaim() {
        return {{0, UNIFORM, sizeof(DefaultTransparentMaterialUniformBlock)}};
    }
} // fmwk