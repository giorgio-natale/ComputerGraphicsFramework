//
// Created by drgio on 16/08/2023.
//

#include "DefaultSprite.h"

namespace fmwk {
    DefaultSprite::DefaultSprite(float brightness) : MaterialComponent(DEFAULT_SPRITE), _brightness(brightness){}

    void DefaultSprite::updateDescriptorSet(int currentImage) {
        DefaultSpriteUniformBlock ubo{};
        ubo.brightness = _brightness;
        _descriptorSet->map(currentImage, &ubo, sizeof(ubo), 0);
    }

    DescriptorSetClaim DefaultSprite::getDescriptorSetClaim() {
        return {{0, UNIFORM, sizeof(DefaultSpriteUniformBlock)}};
    }
} // fmwk