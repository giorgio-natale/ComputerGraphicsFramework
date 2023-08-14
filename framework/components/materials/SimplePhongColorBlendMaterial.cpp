//
// Created by drgio on 14/08/2023.
//

#include "SimplePhongColorBlendMaterial.h"

namespace fmwk {
    SimplePhongColorBlendMaterial::SimplePhongColorBlendMaterial(glm::vec3 color) : MaterialComponent(SIMPLE_PHONG_COLOR_BLEND),
    _color(color),
    _percentage(0.5f){}

    void SimplePhongColorBlendMaterial::updateDescriptorSet(int currentImage) {
        SimplePhongColorBlendMaterialUniformBlock ubo{};
        ubo.color = _color;
        ubo.percentage = _percentage;
        _descriptorSet->map(currentImage, &ubo, sizeof(ubo), 0);
    }

    DescriptorSetClaim SimplePhongColorBlendMaterial::getDescriptorSetClaim() {
        return {{0, UNIFORM, sizeof(SimplePhongColorBlendMaterialUniformBlock)}};
    }
} // fmwk