//
// Created by drgio on 14/08/2023.
//

#ifndef DEMO_SIMPLEPHONGCOLORBLENDMATERIAL_H
#define DEMO_SIMPLEPHONGCOLORBLENDMATERIAL_H

#include "MaterialComponent.h"

namespace fmwk {
    struct SimplePhongColorBlendMaterialUniformBlock{
        alignas(16) glm::vec3 color;
        alignas(4) float percentage;
    };

    class SimplePhongColorBlendMaterial : public MaterialComponent{
    public:
        explicit SimplePhongColorBlendMaterial(glm::vec3 color);

        void updateDescriptorSet(int currentImage) override;

        DescriptorSetClaim getDescriptorSetClaim() override;

        void postUpdate() override;

    private:
        glm::vec3 _color;
        float _percentage;
    };

} // fmwk

#endif //DEMO_SIMPLEPHONGCOLORBLENDMATERIAL_H
