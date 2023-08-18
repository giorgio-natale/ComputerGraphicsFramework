//
// Created by drgio on 16/08/2023.
//

#ifndef DEMO_DEFAULTSPRITE_H
#define DEMO_DEFAULTSPRITE_H

#include "MaterialComponent.h"

namespace fmwk {
    struct DefaultSpriteUniformBlock{
        alignas(4) float brightness;
    };

    class DefaultSprite : public MaterialComponent{
    public:
        explicit DefaultSprite(float brightness);

        void updateDescriptorSet(int currentImage) override;

        DescriptorSetClaim getDescriptorSetClaim() override;

    private:
        float _brightness;
    };

} // fmwk

#endif //DEMO_DEFAULTSPRITE_H
