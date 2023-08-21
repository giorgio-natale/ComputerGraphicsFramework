//
// Created by drgio on 14/08/2023.
//

#include "SimplePhongColorBlendMaterial.h"
#include "../../Entity.h"

namespace fmwk {
    SimplePhongColorBlendMaterial::SimplePhongColorBlendMaterial(glm::vec3 color) : MaterialComponent(SIMPLE_PHONG_COLOR_BLEND),
    _color(color),
    _percentage(0){}

    void SimplePhongColorBlendMaterial::updateDescriptorSet(int currentImage) {
        SimplePhongColorBlendMaterialUniformBlock ubo{};
        ubo.color = _color;
        ubo.percentage = _percentage;
        _descriptorSet->map(currentImage, &ubo, sizeof(ubo), 0);
    }

    DescriptorSetClaim SimplePhongColorBlendMaterial::getDescriptorSetClaim() {
        return {{0, UNIFORM, sizeof(SimplePhongColorBlendMaterialUniformBlock)}};
    }

    void SimplePhongColorBlendMaterial::postUpdate() {
        if(_parentEntity->hasComponent("Health") && _parentEntity->getHealth().isInGracePeriod()) {
            _percentage = 1 - _parentEntity->getHealth().getCurrentLifePercentage();
        }
    }
} // fmwk