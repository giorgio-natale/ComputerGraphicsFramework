//
// Created by drgio on 07/08/2023.
//

#ifndef DEMO_FRAGMENTEDFACE_H
#define DEMO_FRAGMENTEDFACE_H

#include <vector>
#include <sstream>
#include "Position.h"
#include "mazeUtils.h"

namespace mgen {

    class FragmentedFace{
    public:
        FragmentedFace(Position startPosition, Direction const& side, Direction const& growingDirection, int length, float unit):
                _startPosition(startPosition),
                _side(side),
                _growingDirection(growingDirection),
                _length(length),
                _unit(unit),
                _facesIncluded(length, true)
        {}

        [[nodiscard]] std::vector<Rectangle> buildRectangles() const;


        void deleteFace(int faceIndex);

    private:
        Position _startPosition;
        Direction const& _side;
        Direction const& _growingDirection;
        int _length;
        std::vector<bool> _facesIncluded;
        float _unit;

        [[nodiscard]] Rectangle buildRectangle(Position fragmentStartPosition, int fragmentLength) const;
    };

} // mgen

#endif //DEMO_FRAGMENTEDFACE_H
