//
// Created by drgio on 14/08/2023.
//

#include "SimplePhongBlinkMaterial.h"
#include "../../GameEngine.h"

namespace fmwk {
    SimplePhongBlinkMaterial::SimplePhongBlinkMaterial(float frequency, float dutyCycle) : MaterialComponent(
            SIMPLE_PHONG_BLINK),
                                                                                           _frequency(frequency),
                                                                                           _t(0),
                                                                                           _isBlinking(false),
                                                                                           _dutyCycle(dutyCycle) {}

    void SimplePhongBlinkMaterial::updateDescriptorSet(int currentImage) {
        SimplePhongBlinkMaterialUniformBlock ubo{};
        ubo.alpha = 1.0f;
        if (_isBlinking) {
            float period = 1 / _frequency;
            int periodsAlreadyDone = (int) (_t / period);
            float normalizedTime = _t - (float) periodsAlreadyDone * period;
            if (normalizedTime / period > _dutyCycle)
                ubo.alpha = 0;
        }
        _descriptorSet->map(currentImage, &ubo, sizeof(ubo), 0);
    }

    DescriptorSetClaim SimplePhongBlinkMaterial::getDescriptorSetClaim() {
        return {{0, UNIFORM, sizeof(SimplePhongBlinkMaterialUniformBlock)}};
    }

    void SimplePhongBlinkMaterial::update() {
        _t += GameEngine::getInstance()->getInput().deltaTime;
    }

    void SimplePhongBlinkMaterial::postUpdate() {
        if (_parentEntity->hasComponent("Health")) {
            if (_parentEntity->getHealth().isInGracePeriod()) {
                _isBlinking = true;
            } else {
                _isBlinking = false;
            }
        }
    }
} // fmwk