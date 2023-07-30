//
// Created by drgio on 28/07/2023.
//

#ifndef DEMO_DIRECTLIGHTCOMPONENT_H
#define DEMO_DIRECTLIGHTCOMPONENT_H

#include "LightComponent.h"

namespace fmwk {

    class DirectLightComponent : public LightComponent{
    public:
        DirectLightComponent(std::string const& name, glm::vec3 direction, glm::vec4 color);
        void setDirection(glm::vec3 direction);
        void setColor(glm::vec4 color);

        glm::vec3 getDirection();
        glm::vec4 getColor();

        void updateLightUniformBlock(GlobalLightUniformBlock& glubo) override;

    private:
        glm::vec3 _direction;
        glm::vec4 _color;
    };

} // fmwk

#endif //DEMO_DIRECTLIGHTCOMPONENT_H
