//
// Created by drgio on 28/07/2023.
//

#include "SpotLightComponent.h"
#include "systems/RenderSystem.h"
#include "Entity.h"

namespace fmwk {
    SpotLightComponent::SpotLightComponent(const std::string &name, glm::vec4 lightColor, float beta, float g,
                                           float cosOuter, float cosInner) : PointLightComponent(name, lightColor, beta, g),
                                           _lightDir(glm::vec3()), _cosOuter(cosOuter), _cosInner(cosInner){}

    void SpotLightComponent::setCosOuter(float cosOuter) {
        _cosOuter = cosOuter;
    }

    void SpotLightComponent::setCosInner(float cosInner) {
        _cosInner = cosInner;
    }

    void SpotLightComponent::setLightDir(glm::vec3 lightDir) {
        _lightDir = lightDir;
    }

    glm::vec3 SpotLightComponent::getLightDir() const {
        return _lightDir;
    }

    float SpotLightComponent::getCosOuter() const {
        return _cosOuter;
    }

    float SpotLightComponent::getCosInner() const {
        return _cosInner;
    }


    void SpotLightComponent::updateLightUniformBlock(GlobalLightUniformBlock &glubo) {
        if(glubo.spotLightsCount >= SPOT_LIGHTS_MAX)
            throw std::runtime_error("Tried to render more than the maximum amount of spot lights (" + std::to_string(POINT_LIGHTS_MAX) + ")");
        SpotLightBlock spotLightBlock{};
        spotLightBlock.lightColor = getLightColor();
        spotLightBlock.lightPos = _parentEntity->getTransform().getPosition();
        spotLightBlock.beta = getBeta();
        spotLightBlock.g = getG();
        spotLightBlock.lightDir = -(_parentEntity->getTransform().getLocalDirections()[2]);
        spotLightBlock.cosOuter = getCosOuter();
        spotLightBlock.cosInner = getCosInner();

        glubo.spotLights[glubo.spotLightsCount] = spotLightBlock;
        glubo.spotLightsCount++;
    }

} // fmwk