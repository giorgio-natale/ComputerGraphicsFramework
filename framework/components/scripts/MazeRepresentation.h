//
// Created by drgio on 08/08/2023.
//

#ifndef DEMO_MAZEREPRESENTATION_H
#define DEMO_MAZEREPRESENTATION_H

#include <unordered_map>
#include "Component.h"
#include "../../../maze_generation/Maze.h"
#include "../../systems/ModelSystem.h"

namespace fmwk {
    struct DirectionCollisionInfo{
        bool isNear;
        float distance;
    };
    struct Square{
        glm::vec3 bottomLeftCorner;
        float edgeSize;
    };


    class MazeRepresentation : public Component{
    public:
        MazeRepresentation(int rowCount, int colCount, float unit,
                           char **mazeCharRepresentation);

        [[nodiscard]] DirectionCollisionInfo getMazeCollisions(glm::vec3 position, glm::vec3 direction) const;
        [[nodiscard]] std::pair<std::vector<fmwk::VertexWithNormalAndTan>, std::vector<uint32_t>> buildMesh() const;
        [[nodiscard]] std::vector<Square> getBlocksAroundPoint(glm::vec3 point) const;


    private:
        mgen::Maze _maze;
        int _rowCount;
        int _colCount;
        float _unit;
        char** _mazeCharRepresentation;

    };

} // fmwk

#endif //DEMO_MAZEREPRESENTATION_H
