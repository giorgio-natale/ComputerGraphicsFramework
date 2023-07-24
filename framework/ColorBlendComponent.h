//
// Created by drgio on 24/07/2023.
//

#ifndef DEMO_COLORBLENDCOMPONENT_H
#define DEMO_COLORBLENDCOMPONENT_H

#include "MaterialComponent.h"

namespace fmwk {

    struct ColorBlendMaterialUniformBlock{
        alignas(16) glm::vec3 color;
        alignas(4) float percentage;
    };

    class ColorBlendComponent : public MaterialComponent{
    public:
        ColorBlendComponent(const std::string &name, glm::vec3 color, float percentage);

        void updateDescriptorSet(int currentImage) override;

        DescriptorSetClaim getDescriptorSetClaim() override;

    private:
        glm::vec3 _color;
        float _percentage;
    };

} // fmwk

#endif //DEMO_COLORBLENDCOMPONENT_H
