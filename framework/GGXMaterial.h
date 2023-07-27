//
// Created by drgio on 27/07/2023.
//

#ifndef DEMO_GGXMATERIAL_H
#define DEMO_GGXMATERIAL_H


#include "MaterialComponent.h"

namespace fmwk {

    struct GGXUniformBlock{
        alignas(16) glm::vec3 eyePos;
    };
    class GGXMaterial : public MaterialComponent{
    public:
        GGXMaterial(std::string const& name, Texture& normalTexture, Texture& materialTexture);

        void updateDescriptorSet(int currentImage) override;

        DescriptorSetClaim getDescriptorSetClaim() override;

    private:
        Texture& _normalTexture;
        Texture& _materialTexture;
    };

} // fmwk

#endif //DEMO_GGXMATERIAL_H
