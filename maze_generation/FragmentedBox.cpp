//
// Created by drgio on 07/08/2023.
//

#include <vector>
#include "FragmentedBox.h"

namespace mgen {
    FragmentedBox::FragmentedBox(Position startPosition, Direction const& growingDirection, int length, float unit):
    _startPosition(startPosition),
    _endPosition(startPosition + scalar(growingDirection, length-1)),
    _growingDirection(growingDirection),
    _length(length),
    _unit(unit)
    {
        for(auto direction : Direction::directions){
            if(*direction != Direction::BOTTOM){
                if(direction->sameOrientationAs(_growingDirection)){
                    //near face
                    if(direction->isOpposite(_growingDirection)){
                        _map.insert({direction, FragmentedFace(_startPosition, *direction, _growingDirection.getPerpendicularTo(Direction::TOP), 1, (float)unit)});
                    }
                        //far face
                    else{
                        _map.insert({direction, FragmentedFace(_endPosition, *direction, _growingDirection.getPerpendicularTo(Direction::TOP), 1, (float)unit)});
                    }

                }else{
                    // lateral and top faces
                    _map.insert({direction, FragmentedFace(startPosition, *direction, _growingDirection, length, (float)unit)});
                }
            }
        }
    }

    [[nodiscard]] std::vector<Rectangle> FragmentedBox::buildRectangles() const{
        auto rectangles = std::vector<Rectangle>();
        for(const auto& [direction, fragmentFace] : _map)
            for (const auto& rectangle : fragmentFace.buildRectangles())
                rectangles.push_back(rectangle);
        return rectangles;
    }

    void FragmentedBox::deleteFace(Position position, Direction const& side){
        int deltaCol = position.getCol() - _startPosition.getCol();
        int deltaRow = position.getRow() - _startPosition.getRow();
        if(deltaCol != 0 && deltaRow != 0){
            return;
        }

        int distance = (deltaCol == 0) ? deltaRow : deltaCol;
        distance = abs(distance);

        if(_startPosition + scalar(_growingDirection, distance) != position){
            return;
        }

        if(distance >= _length){
            return;
        }

        if(side == _growingDirection) {
            if (distance != _length - 1) {
                return;
            }
            _map.find(&side)->second.deleteFace(0);
            return;
        }

        if(side.isOpposite(_growingDirection)){
            if(distance != 0){
                return;
            }
            _map.find(&side)->second.deleteFace(0);
            return;
        }

        _map.find(&side)->second.deleteFace(distance);

    }
} // mgen