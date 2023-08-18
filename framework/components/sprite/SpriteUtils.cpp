//
// Created by drgio on 17/08/2023.
//

#include "SpriteUtils.h"

namespace fmwk {

    bool doesSideDefineWidth(UiSide side){
        return side == LEFT || side == RIGHT;
    }

    UiSide getOppositeSide(UiSide side){
        switch (side) {
            case TOP:
                return BOTTOM;
            case BOTTOM:
                return TOP;
            case LEFT:
                return RIGHT;
            case RIGHT:
                return LEFT;
            default:
                return TOP;
        }
    }



    UiRectangle::UiRectangle(glm::vec2 rectangleOriginalSize, glm::vec2 virtualScreenSize,
                             const std::vector<UiAnchor>& anchors, bool stretchable):
                             _rectangleOriginalSize(rectangleOriginalSize),
                             _virtualScreenSize(virtualScreenSize),
                             _sides({}),
                             _stretchable(stretchable){

        _sides.insert({TOP, {TOP, _virtualScreenSize.y, false}});
        _sides.insert({BOTTOM, {BOTTOM, 0, false}});
        _sides.insert({LEFT, {LEFT, 0, false}});
        _sides.insert({RIGHT, {RIGHT, _virtualScreenSize.x, false}});


        for(auto const& anchor : anchors){
            auto &elem = _sides.find(anchor.side)->second;
            elem.setSize(toBottomLeftOffset(anchor.side, anchor.offset));
        }

        for(auto &[side, rectangleSide] : _sides){
            if(!rectangleSide.decided){
                if(anchors.size() == 2 || anchors.size() == 3 && _stretchable){
                    float distanceBetweenSides = doesSideDefineWidth(side) ? _rectangleOriginalSize.x : _rectangleOriginalSize.y;
                    rectangleSide.setSize(computeOffset(rectangleSide.offset, _sides.find(getOppositeSide(side))->second.offset, distanceBetweenSides));
                }else{
                    std::vector<float> adjacentOffsets{};
                    for(auto& [side1, rectangleSide1] : _sides)
                        if(getOppositeSide(side) != side1)
                            adjacentOffsets.push_back(rectangleSide1.offset);
                    float distanceBetweenAdjacentSides = abs(adjacentOffsets[0] - adjacentOffsets[1]);
                    float distanceBetweenSides = doesSideDefineWidth(side)
                            ? distanceBetweenAdjacentSides * _rectangleOriginalSize.x / _rectangleOriginalSize.y
                            : distanceBetweenAdjacentSides * _rectangleOriginalSize.y / _rectangleOriginalSize.x;
                    rectangleSide.setSize(computeOffset(rectangleSide.offset, _sides.find(getOppositeSide(side))->second.offset, distanceBetweenSides));
                }
            }
        }


    }

    float UiRectangle::computeOffset(float srcOffset, float dstOffset, float desiredSpaceBetweenSides) {
        float shrinkingDirection = 1.0f;
        if (srcOffset >= dstOffset)
            shrinkingDirection = -1.0f;

        float currentDistanceBetweenSides = abs(dstOffset - srcOffset);
        float shrinkingDistance = currentDistanceBetweenSides - desiredSpaceBetweenSides;
        return srcOffset + shrinkingDirection * shrinkingDistance;
    }

    std::vector<glm::vec2> UiRectangle::getRectangleVertices() {
        return {
            {_sides.find(LEFT)->second.offset, _sides.find(BOTTOM)->second.offset},
            {_sides.find(RIGHT)->second.offset, _sides.find(BOTTOM)->second.offset},
            {_sides.find(RIGHT)->second.offset, _sides.find(TOP)->second.offset},
            {_sides.find(LEFT)->second.offset, _sides.find(TOP)->second.offset}
        };
    }

    float UiRectangle::toBottomLeftOffset(UiSide side, float naturalOffset) const{
        if(side == TOP)
            return _virtualScreenSize.y - naturalOffset;
        if(side == RIGHT)
            return _virtualScreenSize.x - naturalOffset;
        return naturalOffset;
    }
} // fmwk