//
// Created by drgio on 07/08/2023.
//

#include "MazeCell.h"

namespace mgen {
    MazeCell::MazeCell(char content, std::map<Direction const*, char> const& neighbours, Position position): _position(position){
        _isBlock = content == '#';
        if(_isBlock) {
            for (auto const &[direction, value]: neighbours) {
                if (value == '#')
                    _validDirections.insert({direction});
            }
        }
    }

    bool MazeCell::isBlock() const{
        return _isBlock;
    }

    Position MazeCell::getPosition() const{
        return _position;
    }

    std::unordered_set<Direction const*> MazeCell::getValidDirections() const{
        return _validDirections;
    }

    void MazeCell::addBoxToNeighbours(Direction const& direction, FragmentedBox& box){
        _neighbourBoxes.insert({&direction, &box});
    }

    std::map<Direction const*, FragmentedBox*> MazeCell::getNeighbourBoxes() const{
        return _neighbourBoxes;
    };
} // mgen