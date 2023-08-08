//
// Created by drgio on 07/08/2023.
//

#include <vector>
#include <map>
#include <sstream>
#include "Direction.h"
#include "Position.h"

namespace mgen {
    const Direction Direction::NORTH = Direction(0, 1, 0);
    const Direction Direction::SOUTH = Direction(0, -1, 0);
    const Direction Direction::EAST = Direction(1, 0, 0);
    const Direction Direction::WEST = Direction(-1, 0, 0);
    const Direction Direction::TOP = Direction(0, 0, 1);
    const Direction Direction::BOTTOM = Direction(0, 0, -1);

    const std::vector<Direction const*> directions =
            {&Direction::NORTH, &Direction::SOUTH, &Direction::EAST, &Direction::WEST, &Direction::TOP, &Direction::BOTTOM};

    const std::map<std::vector<int>, Direction const*> directionByVector = {
            {std::vector<int>{0,1,0}, &Direction::NORTH},
            {std::vector<int>{0,-1,0}, &Direction::SOUTH},
            {std::vector<int>{1,0,0}, &Direction::EAST},
            {std::vector<int>{-1,0,0}, &Direction::WEST},
            {std::vector<int>{0,0,1}, &Direction::TOP},
            {std::vector<int>{0,0,-1}, &Direction::BOTTOM}
    };

    Direction const& directionOf(const std::vector<int>& vec){
        auto itr = directionByVector.find(vec);
        if(itr == directionByVector.end()) {
            std::ostringstream oss;
            oss << "Cannot find a direction associated to the vector (" << vec[0] << ", " << vec[1] << ", " << vec[2] << ")";
            throw std::runtime_error(oss.str());
        }
        return *directionByVector.find(vec)->second;
    }


    Direction const &Direction::getPerpendicularTo(const Direction &other) const {
        auto& a = _direction;
        auto& b = other._direction;
        auto result = std::vector<int>{a[1]*b[2] - a[2]*b[1], -1*(a[0]*b[2] - a[2]*b[0]), a[0]*b[1] - a[1]*b[0]};
        return directionOf(result);
    }

    Direction const &Direction::getOpposite() const {
        std::vector<int> result;
        for(int i : _direction)
            result.push_back(i * -1);
        return directionOf(result);
    }

    glm::vec3 toGlm(Direction const& direction, float unit){
        Position pos = Position(direction.getDirection()[0], direction.getDirection()[1], direction.getDirection()[2]);
        return toGlm(pos, unit);
    }

    const std::vector<Direction const*> Direction::directions =
            {&Direction::NORTH, &Direction::SOUTH, &Direction::EAST, &Direction::WEST, &Direction::TOP, &Direction::BOTTOM};
} // mgen