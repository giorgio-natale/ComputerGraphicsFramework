//
// Created by drgio on 26/07/2023.
//

#include "SimplePhongMaterial.h"
#include "GameEngine.h"

namespace fmwk {
    SimplePhongMaterial::SimplePhongMaterial(const std::string &name) : MaterialComponent(name, SIMPLE_PHONG) {}
    void SimplePhongMaterial::updateDescriptorSet(int currentImage) {
        SimplePhongUniformBlock ubo{};
        ubo.eyePos = GameEngine::getInstance()->getEntityByName("Camera").getTransform().getPosition();
        _descriptorSet->map(currentImage, &ubo, sizeof(ubo), 0);
    }

    DescriptorSetClaim SimplePhongMaterial::getDescriptorSetClaim() {
        return {{0, UNIFORM, sizeof(SimplePhongUniformBlock)}};
    }

} // fmwk