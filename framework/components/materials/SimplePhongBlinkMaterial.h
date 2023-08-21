//
// Created by drgio on 14/08/2023.
//

#ifndef DEMO_SIMPLEPHONGBLINKMATERIAL_H
#define DEMO_SIMPLEPHONGBLINKMATERIAL_H

#include "MaterialComponent.h"

namespace fmwk {

    struct SimplePhongBlinkMaterialUniformBlock{
        alignas(4) float alpha;
    };

    class SimplePhongBlinkMaterial : public MaterialComponent{
    public:
        SimplePhongBlinkMaterial(float frequency, float dutyCycle);

        void updateDescriptorSet(int currentImage) override;

        DescriptorSetClaim getDescriptorSetClaim() override;

        void update() override;

        void postUpdate() override;

    private:
        float _frequency;
        float _dutyCycle;
        float _t;
        bool _isBlinking;
    };

} // fmwk

#endif //DEMO_SIMPLEPHONGBLINKMATERIAL_H
