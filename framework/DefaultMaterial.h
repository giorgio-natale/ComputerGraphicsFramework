//
// Created by drgio on 20/07/2023.
//

#ifndef DEMO_DEFAULTMATERIAL_H
#define DEMO_DEFAULTMATERIAL_H

#include "MaterialComponent.h"

namespace fmwk {
    struct DefaultMaterialUniformBlock{
        alignas(4) float brightness;
    };

    class DefaultMaterial : public MaterialComponent{
    public:
        DefaultMaterial(const std::string &name, float brightness);
        void updateDescriptorSet(int currentImage) override;
        DescriptorSetClaim getDescriptorSetClaim() override;
    private:
        float _brightness;
    };

} // fmwk

#endif //DEMO_DEFAULTMATERIAL_H
