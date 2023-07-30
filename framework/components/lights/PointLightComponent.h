//
// Created by drgio on 28/07/2023.
//

#ifndef DEMO_POINTLIGHTCOMPONENT_H
#define DEMO_POINTLIGHTCOMPONENT_H

#include "LightComponent.h"

namespace fmwk {

    class PointLightComponent : public LightComponent{
    public:
        PointLightComponent(std::string const& name, glm::vec4 lightColor, float beta, float g);

        void setLightColor(glm::vec4 lightColor);
        void setBeta(float beta);
        void setG(float g);

        glm::vec4 getLightColor();
        [[nodiscard]] float getBeta() const;
        [[nodiscard]] float getG() const;

        void updateLightUniformBlock(GlobalLightUniformBlock &glubo) override;

    private:
        glm::vec4 _lightColor;
        float _beta;
        float _g;
    };

} // fmwk

#endif //DEMO_POINTLIGHTCOMPONENT_H
