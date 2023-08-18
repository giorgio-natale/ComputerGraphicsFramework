//
// Created by drgio on 16/08/2023.
//

#include "Sprite.h"
#include "SpriteUtils.h"
#include <glm/gtx/matrix_transform_2d.hpp>


namespace fmwk {

    Sprite::Sprite(glm::vec2 preferredSize, const std::vector<UiAnchor>& anchors, bool stretchable) : Component("Sprite"),

        _alreadyProvisioned(false),
        _descriptorSet(nullptr),
        _preferredSize(preferredSize),
        _anchors(anchors),
        _stretchable(stretchable)
        {}

    bool Sprite::isProvisioned() const {
        return _alreadyProvisioned;
    }

    void Sprite::provision(DescriptorSet *descriptorSet) {
        _descriptorSet = descriptorSet;
        _alreadyProvisioned = true;
    }

    DescriptorSet &Sprite::getDescriptorSet() {
        return *_descriptorSet;
    }

    void Sprite::updateDescriptorSet(int currentImage, float virtualScreenWidthUnits, float aspectRatio) {
        SpriteUniformBlock subo{};

        UiRectangle transformedRectangle = UiRectangle(_preferredSize, {virtualScreenWidthUnits, virtualScreenWidthUnits/aspectRatio}, _anchors, _stretchable);
        auto targetPoints = transformedRectangle.getRectangleVertices();

        float horizontalScale = targetPoints[1].x - targetPoints[0].x;
        float verticalScale = targetPoints[3].y - targetPoints[0].y;

        glm::vec2 translation = targetPoints[0];

        subo.spriteTransform = glm::mat4(
                glm::translate(glm::mat3(1), translation) * glm::scale(glm::mat3(1), {horizontalScale, verticalScale})
        );
        _descriptorSet->map(currentImage, &subo, sizeof(subo), 0);
    }

} // fmwk