//
// Created by drgio on 28/07/2023.
//

#ifndef DEMO_SPOTLIGHTCOMPONENT_H
#define DEMO_SPOTLIGHTCOMPONENT_H

#include "PointLightComponent.h"

namespace fmwk {

    class SpotLightComponent : public PointLightComponent{
    public:
        SpotLightComponent(std::string const& name, glm::vec4 lightColor, float beta, float g, float cosOuter, float cosInner);

        void setCosOuter(float cosOuter);
        void setCosInner(float cosInner);
        void setLightDir(glm::vec3 lightDir);

        [[nodiscard]] float getCosOuter() const;
        [[nodiscard]] float getCosInner() const;
        [[nodiscard]] glm::vec3 getLightDir() const;

    private:
        void updateLightUniformBlock(GlobalLightUniformBlock &glubo) override;

    private:
        float _cosOuter;
        float _cosInner;
        glm::vec3 _lightDir;
    };

} // fmwk

#endif //DEMO_SPOTLIGHTCOMPONENT_H
