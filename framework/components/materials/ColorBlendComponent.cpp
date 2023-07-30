//
// Created by drgio on 24/07/2023.
//

#include "ColorBlendComponent.h"

namespace fmwk {
    ColorBlendComponent::ColorBlendComponent(glm::vec3 color, float percentage) : MaterialComponent(COLOR_BLEND), _color(color), _percentage(percentage)
    {}

    void ColorBlendComponent::updateDescriptorSet(int currentImage) {
        ColorBlendMaterialUniformBlock ubo{};
        ubo.percentage = _percentage;
        ubo.color = _color;
        _descriptorSet->map(currentImage, &ubo, sizeof(ubo), 0);

    }

    DescriptorSetClaim ColorBlendComponent::getDescriptorSetClaim() {
        return {{0, UNIFORM, sizeof(ColorBlendMaterialUniformBlock)}};
    }


} // fmwk