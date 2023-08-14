//
// Created by drgio on 14/08/2023.
//

#include "SimplePhongBlinkMaterial.h"
#include "../../GameEngine.h"

namespace fmwk {
    SimplePhongBlinkMaterial::SimplePhongBlinkMaterial(float frequency) : MaterialComponent(SIMPLE_PHONG_BLINK),
                                                                          _frequency(frequency),
                                                                          _t(0),
                                                                          _isBlinking(false){}

    void SimplePhongBlinkMaterial::updateDescriptorSet(int currentImage) {
        SimplePhongBlinkMaterialUniformBlock ubo{};
        ubo.frequency = _frequency;
        ubo.t = _t;
        ubo.isBlinking = _isBlinking;
        _descriptorSet->map(currentImage, &ubo, sizeof(ubo), 0);
    }

    DescriptorSetClaim SimplePhongBlinkMaterial::getDescriptorSetClaim() {
        return {{0, UNIFORM, sizeof(SimplePhongBlinkMaterialUniformBlock)}};
    }

    void SimplePhongBlinkMaterial::update() {
        _t += GameEngine::getInstance()->getInput().deltaTime;
    }
} // fmwk