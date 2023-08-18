//
// Created by drgio on 17/08/2023.
//

#ifndef DEMO_SPRITEUTILS_H
#define DEMO_SPRITEUTILS_H

#include <unordered_map>
#include <vector>
#include <glm/vec2.hpp>

namespace fmwk {

    enum UiSide{TOP, BOTTOM, LEFT, RIGHT};

    bool doesSideDefineWidth(UiSide side);
    UiSide getOppositeSide(UiSide side);

    struct UiAnchor{
        UiSide side;
        float offset;
    };

    struct UiRectangleSide{
        UiSide side;
        float offset; //always relative to bottom/left
        bool decided;

        inline void setSize(float assignedOffset){
            offset = assignedOffset;
            decided = true;
        }

    };

    class UiRectangle{
    public:
        UiRectangle(glm::vec2 rectangleOriginalSize, glm::vec2 virtualScreenSize, const std::vector<UiAnchor>& anchors, bool stretchable);
        std::vector<glm::vec2> getRectangleVertices();
    private:
        glm::vec2 _rectangleOriginalSize;
        glm::vec2 _virtualScreenSize;
        std::unordered_map<UiSide, UiRectangleSide> _sides;
        bool _stretchable;

        static float computeOffset(float srcOffset, float dstOffset, float desiredSpaceBetweenSides);
        float toBottomLeftOffset(UiSide side, float naturalOffset) const;

    };



} // fmwk

#endif //DEMO_SPRITEUTILS_H
