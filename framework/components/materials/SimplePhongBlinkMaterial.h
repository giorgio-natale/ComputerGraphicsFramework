//
// Created by drgio on 14/08/2023.
//

#ifndef DEMO_SIMPLEPHONGBLINKMATERIAL_H
#define DEMO_SIMPLEPHONGBLINKMATERIAL_H

#include "MaterialComponent.h"

namespace fmwk {

    struct SimplePhongBlinkMaterialUniformBlock{
        alignas(4) bool isBlinking;
        alignas(4) float frequency;
        alignas(4) float t;
    };

    class SimplePhongBlinkMaterial : public MaterialComponent{
    public:
        explicit SimplePhongBlinkMaterial(float frequency);

        void updateDescriptorSet(int currentImage) override;

        DescriptorSetClaim getDescriptorSetClaim() override;

        void update() override;

    private:
        float _frequency;
        float _t;
        bool _isBlinking;
    };

} // fmwk

#endif //DEMO_SIMPLEPHONGBLINKMATERIAL_H
