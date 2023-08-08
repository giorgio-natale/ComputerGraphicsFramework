//
// Created by drgio on 07/08/2023.
//

#ifndef DEMO_DIRECTION_H
#define DEMO_DIRECTION_H

#include <array>
#include <glm/vec3.hpp>
#include <vector>
namespace mgen {

    class Direction {
    public:
        static const Direction NORTH;
        static const Direction SOUTH;
        static const Direction EAST;
        static const Direction WEST;
        static const Direction TOP;
        static const Direction BOTTOM;

        static const std::vector<Direction const*> directions;

        [[nodiscard]] bool sameOrientationAs(Direction const& other) const{
            return _direction[0] * other._direction[0] + _direction[1] * other._direction[1] + _direction[2] * other._direction[2];
        }

        [[nodiscard]] bool isOpposite(Direction const& other) const{
            return _direction[0] * other._direction[0] + _direction[1] * other._direction[1] + _direction[2] * other._direction[2] == -1;
        }

        [[nodiscard]] std::array<int, 3> const& getDirection() const{
            return _direction;
        }
        [[nodiscard]] Direction const& getOpposite() const;
        [[nodiscard]] Direction const& getPerpendicularTo(Direction const& other) const;

        [[nodiscard]] bool operator==(Direction const& dir) const{
            return _direction == dir._direction;
        };
        [[nodiscard]] bool operator!=(Direction const &dir) const{
            return !(_direction == dir._direction);
        };

    private:
        std::array<int, 3> _direction{};
        int dot(Direction const& other){
            return _direction[0] * other._direction[0] + _direction[1] * other._direction[1] + _direction[2] * other._direction[2];
        }
        Direction(int x, int y, int z){
            _direction[0] = x;
            _direction[1] = y;
            _direction[2] = z;
        }
    };

    glm::vec3 toGlm(Direction const& direction, float unit);


} // mgen

#endif //DEMO_DIRECTION_H
