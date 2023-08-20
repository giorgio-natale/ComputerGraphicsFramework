//
// Created by drgio on 07/08/2023.
//

#ifndef DEMO_MAZE_H
#define DEMO_MAZE_H

#include <vector>
#include <memory>
#include "MazeCell.h"

namespace mgen {

    class Maze{
    public:
        Maze();

        Maze(std::vector<std::vector<char>> const& matrix, int rowCount, int colCount, float unit);
        void buildBoxes();

        void deleteUselessFaces();

        [[nodiscard]] std::vector<FragmentedBox const*> getBoxes () const;

        [[nodiscard]] bool isValidPosition(Position position) const;
        [[nodiscard]] bool isBlock(Position position) const;
        [[nodiscard]] bool isEmpty(Position position) const;
        [[nodiscard]] MazeCell& getCell(Position position) const;
        [[nodiscard]] int getWidth() const;
        [[nodiscard]] int getHeight() const;


    private:
        std::vector<std::vector<std::unique_ptr<MazeCell>>> _matrix;
        std::vector<std::unique_ptr<FragmentedBox>> _boxes;
        unsigned int _width;
        unsigned int _height;
        float _unit;
        [[nodiscard]] std::vector<std::vector<MazeCell*>> getSlices(Position startPosition, Direction const& direction) const;

    };

} // mgen

#endif //DEMO_MAZE_H
