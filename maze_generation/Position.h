//
// Created by drgio on 07/08/2023.
//

#ifndef DEMO_POSITION_H
#define DEMO_POSITION_H

#include <glm/vec3.hpp>
#include "Direction.h"

namespace mgen {

    class Position{
    public:
        Position(int col, int row, int height){
            _position[0] = col;
            _position[1] = row;
            _position[2] = height;
        }

        Position operator+(Direction const& direction) const{
            return {_position[0] + direction.getDirection()[0], _position[1] + direction.getDirection()[1], _position[2] + direction.getDirection()[2]};
        }

        Position operator+(Position const& position) const{
            return {_position[0] + position[0], _position[1] + position[1], _position[2] + position[2]};
        }

        Position operator-(Direction const& direction) const{
            return {_position[0] - direction.getDirection()[0], _position[1] - direction.getDirection()[1], _position[2] - direction.getDirection()[2]};
        }

        Position operator-(Position const& position) const{
            return {_position[0] - position[0], _position[1] - position[1], _position[2] - position[2]};
        }

        bool operator==(Position const& other) const{
            return _position == other._position;
        }

        bool operator!=(Position const& other) const{
            return _position != other._position;
        }

        int operator[](int index) const{
            return _position[index];
        }
        [[nodiscard]] int getRow() const{
            return _position[1];
        }
        [[nodiscard]] int getCol() const{
            return _position[0];
        }

        [[nodiscard]] std::array<int, 3> getPosition() const{
            return _position;
        }
    private:
        std::array<int, 3> _position{};
    };



    Position scalar(Direction const& dir, int scalar);
    glm::vec3 toGlm(Position position, float unit);

} // mgen

#endif //DEMO_POSITION_H
