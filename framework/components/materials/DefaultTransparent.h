//
// Created by drgio on 14/08/2023.
//

#ifndef DEMO_DEFAULTTRANSPARENT_H
#define DEMO_DEFAULTTRANSPARENT_H

#include "MaterialComponent.h"

namespace fmwk {
    struct DefaultTransparentMaterialUniformBlock{
        alignas(4) float brightness;
        alignas(4) float transparency;
    };

    class DefaultTransparent : public MaterialComponent{
    public:
        DefaultTransparent(float brightness, float transparency);

        void updateDescriptorSet(int currentImage) override;

        DescriptorSetClaim getDescriptorSetClaim() override;

    private:
        float _brightness;
        float _transparency;
    };

} // fmwk

#endif //DEMO_DEFAULTTRANSPARENT_H
