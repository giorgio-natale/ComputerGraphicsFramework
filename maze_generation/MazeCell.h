//
// Created by drgio on 07/08/2023.
//

#ifndef DEMO_MAZECELL_H
#define DEMO_MAZECELL_H

#include <unordered_set>
#include <map>
#include "Direction.h"
#include "Position.h"
#include "FragmentedBox.h"

namespace mgen {

    class MazeCell{
    public:
        explicit MazeCell(char content, std::map<Direction const*, char> const& neighbours, Position position);

        bool isBlock() const;

        Position getPosition() const;

        std::unordered_set<Direction const*> getValidDirections() const;

        void addBoxToNeighbours(Direction const& direction, FragmentedBox& box);

        std::map<Direction const*, FragmentedBox*> getNeighbourBoxes() const;

        virtual ~MazeCell()= default;
    private:
        std::unordered_set<Direction const*> _validDirections;
        std::map<Direction const*, FragmentedBox*> _neighbourBoxes;
        bool _isBlock;
        Position _position;
    };

} // mgen

#endif //DEMO_MAZECELL_H
