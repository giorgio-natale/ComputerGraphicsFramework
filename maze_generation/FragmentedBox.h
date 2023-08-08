//
// Created by drgio on 07/08/2023.
//

#ifndef DEMO_FRAGMENTEDBOX_H
#define DEMO_FRAGMENTEDBOX_H

#include <map>
#include "Position.h"
#include "FragmentedFace.h"

namespace mgen {

    class FragmentedBox{
    public:
        FragmentedBox(Position startPosition, Direction const& growingDirection, int length, float unit);

        [[nodiscard]] std::vector<Rectangle> buildRectangles() const;

        void deleteFace(Position position, Direction const& side);

    private:
        std::map<Direction const*, FragmentedFace> _map;
        Position _startPosition;
        Position _endPosition;
        Direction const& _growingDirection;
        int _length;
        float _unit;
    };

} // mgen

#endif //DEMO_FRAGMENTEDBOX_H
