//
// Created by drgio on 20/07/2023.
//

#include "DefaultMaterial.h"
#include "../../GameEngine.h"
#include "../mesh/MeshComponent.h"
#include "../texture/TextureComponent.h"

namespace fmwk {
    void DefaultMaterial::updateDescriptorSet(int currentImage) {
        DefaultMaterialUniformBlock ubo{};
        ubo.brightness = _brightness;
        _descriptorSet->map(currentImage, &ubo, sizeof(ubo), 0);
    }

    DescriptorSetClaim DefaultMaterial::getDescriptorSetClaim() {
        return {{0, UNIFORM, sizeof(DefaultMaterialUniformBlock)}};
    }

    DefaultMaterial::DefaultMaterial(float brightness) : MaterialComponent(DEFAULT), _brightness(brightness){}
} // fmwk