//
// Created by drgio on 28/07/2023.
//

#include "DirectLightComponent.h"
#include "../../systems/RenderSystem.h"

namespace fmwk {
    DirectLightComponent::DirectLightComponent(const std::string &name, glm::vec3 direction, glm::vec4 color) : LightComponent(name), _direction(direction), _color(color) {}

    void DirectLightComponent::setDirection(glm::vec3 direction) {
        _direction = direction;
    }

    void DirectLightComponent::setColor(glm::vec4 color) {
        _color = color;
    }

    glm::vec3 DirectLightComponent::getDirection() {
        return _direction;
    }

    glm::vec4 DirectLightComponent::getColor() {
        return _color;
    }

    void DirectLightComponent::updateLightUniformBlock(GlobalLightUniformBlock& glubo) {
        if(glubo.directLightsCount >= DIRECT_LIGHTS_MAX)
            throw std::runtime_error("Tried to render more than the maximum amount of direct lights (" + std::to_string(DIRECT_LIGHTS_MAX) + ")");
        DirectLightBlock directLightBlock{};
        directLightBlock.lightDir = _direction;
        directLightBlock.lightColor = _color;
        glubo.directLights[glubo.directLightsCount] = directLightBlock;
        glubo.directLightsCount++;
    }

} // fmwk