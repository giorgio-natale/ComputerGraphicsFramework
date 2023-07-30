//
// Created by drgio on 28/07/2023.
//

#ifndef DEMO_LIGHTCOMPONENT_H
#define DEMO_LIGHTCOMPONENT_H

#include "../scripts/Component.h"
#include "../../../Starter.hpp"


namespace fmwk {

    struct GlobalLightUniformBlock;
    class LightComponent : public Component{
    public:
        explicit LightComponent(std::string const& name);
        [[nodiscard]] bool isProvisioned() const override;
        void provision(DescriptorSet* descriptorSet);
        virtual void updateLightUniformBlock(GlobalLightUniformBlock& glubo) = 0;
        void turnOn();
        void turnOff();
        [[nodiscard]] bool isTurnedOn() const;

    protected:
        DescriptorSet* _descriptorSet;
    private:
        bool _alreadyProvisioned;
        bool _isTurnedOn;
    };

} // fmwk

#endif //DEMO_LIGHTCOMPONENT_H
