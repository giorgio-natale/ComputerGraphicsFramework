//
// Created by drgio on 26/07/2023.
//

#ifndef DEMO_SIMPLEPHONGMATERIAL_H
#define DEMO_SIMPLEPHONGMATERIAL_H

#include "MaterialComponent.h"

namespace fmwk {

    struct SimplePhongUniformBlock{
        alignas(16) glm::vec3 eyePos;
    };

    class SimplePhongMaterial : public MaterialComponent{
    public:
        explicit SimplePhongMaterial(std::string const& name);

        void updateDescriptorSet(int currentImage) override;

        DescriptorSetClaim getDescriptorSetClaim() override;
    };

} // fmwk

#endif //DEMO_SIMPLEPHONGMATERIAL_H
