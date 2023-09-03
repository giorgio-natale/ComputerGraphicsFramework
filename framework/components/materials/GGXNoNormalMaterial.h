//
// Created by drgio on 03/09/2023.
//

#ifndef DEMO_GGXNONORMALMATERIAL_H
#define DEMO_GGXNONORMALMATERIAL_H

#include "MaterialComponent.h"

namespace fmwk {

    struct GGXNoNormalUniformBlock{
        alignas(16) glm::vec3 eyePos;
    };
    class GGXNoNormalMaterial : public MaterialComponent{
    public:
        explicit GGXNoNormalMaterial(Texture &materialTexture);

        void updateDescriptorSet(int currentImage) override;

        DescriptorSetClaim getDescriptorSetClaim() override;

    private:
        Texture& _materialTexture;
    };

} // fmwk

#endif //DEMO_GGXNONORMALMATERIAL_H
