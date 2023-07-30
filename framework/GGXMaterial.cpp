//
// Created by drgio on 27/07/2023.
//

#include "GGXMaterial.h"
#include "GameEngine.h"

namespace fmwk {
    GGXMaterial::GGXMaterial(Texture &normalTexture, Texture &materialTexture)
        : MaterialComponent(GGX), _normalTexture(normalTexture), _materialTexture(materialTexture){}

    void GGXMaterial::updateDescriptorSet(int currentImage) {
        GGXUniformBlock ubo{};
        ubo.eyePos = GameEngine::getInstance()->getEntityByName("Camera").getTransform().getPosition();
        _descriptorSet->map(currentImage, &ubo, sizeof(ubo), 0);
    }

    DescriptorSetClaim GGXMaterial::getDescriptorSetClaim() {
        return {
            {0, UNIFORM, sizeof(GGXUniformBlock)},
            {1, TEXTURE, 0, &_normalTexture},
            {2, TEXTURE, 0, &_materialTexture}
        };

    }
} // fmwk