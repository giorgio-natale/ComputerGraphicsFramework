//
// Created by drgio on 03/09/2023.
//

#include "GGXNoNormalMaterial.h"
#include "../../GameEngine.h"

namespace fmwk {
    GGXNoNormalMaterial::GGXNoNormalMaterial(Texture &materialTexture) : MaterialComponent(
            GGX_NO_NORMAL), _materialTexture(materialTexture) {}

    void GGXNoNormalMaterial::updateDescriptorSet(int currentImage) {
        GGXNoNormalUniformBlock ubo{};
        ubo.eyePos = GameEngine::getInstance()->getEntityByName("Camera").getTransform().getPosition();
        _descriptorSet->map(currentImage, &ubo, sizeof(ubo), 0);
    }

    DescriptorSetClaim GGXNoNormalMaterial::getDescriptorSetClaim() {
        return {
                {0, UNIFORM, sizeof(GGXNoNormalUniformBlock)},
                {1, TEXTURE, 0, &_materialTexture}
        };
    }
} // fmwk