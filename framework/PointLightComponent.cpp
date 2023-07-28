//
// Created by drgio on 28/07/2023.
//

#include "PointLightComponent.h"
#include "systems/RenderSystem.h"
#include "Entity.h"

namespace fmwk {
    PointLightComponent::PointLightComponent(std::string const& name, glm::vec4 lightColor, float beta, float g) :LightComponent(name),
                                             _lightColor(lightColor), _beta(beta), _g(g) {}


    void PointLightComponent::setLightColor(glm::vec4 lightColor) {
        _lightColor = lightColor;
    }

    void PointLightComponent::setBeta(float beta) {
        _beta = beta;
    }

    void PointLightComponent::setG(float g) {
        _g = g;
    }

    glm::vec4 PointLightComponent::getLightColor() {
        return _lightColor;
    }

    float PointLightComponent::getBeta() const {
        return _beta;
    }

    float PointLightComponent::getG() const {
        return _g;
    }

    void PointLightComponent::updateLightUniformBlock(GlobalLightUniformBlock &glubo) {
        if(glubo.pointLightsCount >= POINT_LIGHTS_MAX)
            throw std::runtime_error("Tried to render more than the maximum amount of point lights (" + std::to_string(POINT_LIGHTS_MAX) + ")");
        PointLightBlock pointLightBlock{};
        pointLightBlock.lightColor = _lightColor;
        pointLightBlock.lightPos = _parentEntity->getTransform().getPosition();
        pointLightBlock.beta = _beta;
        pointLightBlock.g = _g;

        glubo.pointLights[glubo.pointLightsCount] = pointLightBlock;
        glubo.pointLightsCount++;
    }

} // fmwk