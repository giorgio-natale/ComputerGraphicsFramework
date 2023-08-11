//
// Created by drgio on 08/08/2023.
//

#include "MazeRepresentation.h"
#include "../../Entity.h"
#include "../../../maze_generation/VertexDictionary.h"

namespace fmwk {
    MazeRepresentation::MazeRepresentation(int rowCount, int colCount, float unit,
                                           char **mazeCharRepresentation) : Component("MazeRepresentation"), _rowCount(rowCount),
                                                                            _colCount(colCount), _unit(unit),
                                                                            _mazeCharRepresentation(
                                                                                    mazeCharRepresentation) {

        auto vec = std::vector<std::vector<char>>(_rowCount, std::vector<char>(_colCount, '#'));
        for(int i = 0; i < _rowCount; i++){
            for(int j = 0; j < _colCount; j++){
                vec[i][j] = _mazeCharRepresentation[_rowCount-1-i][j];
            }
        }
        _maze = mgen::Maze(vec, rowCount, colCount, _unit);
        _maze.buildBoxes();
        _maze.deleteUselessFaces();

    }

    DirectionCollisionInfo MazeRepresentation::getMazeCollisions(glm::vec3 position, glm::vec3 direction) const{
        mgen::Position mazePosition = mgen::fromGlm(position, _unit);
        mgen::Position mazeDirection = mgen::fromGlm(direction, 1.0f);
        mgen::Position targetPosition = mazePosition + mazeDirection;

        if(!_maze.isBlock(targetPosition)){
            return {false, 0};
        }

        auto centerOfTargetBlock = mgen::toGlm(targetPosition, _unit) + _unit * glm::vec3(0.5f, 0.5f, -0.5f);
        auto targetWallPosition = centerOfTargetBlock - 0.5f * _unit * direction;
        float distance = glm::length(glm::dot(targetWallPosition, direction) - glm::dot(position, direction));
        return {true, distance};
    }

    std::pair<std::vector<fmwk::VertexWithNormal>, std::vector<uint32_t>> MazeRepresentation::buildMesh() const {
        auto vertexDict = mgen::VertexDictionary();
        for(auto const& box : _maze.getBoxes()){
            for(auto& r : box->buildRectangles()){
                vertexDict.addRectangle(r);
            }
        }

        std::vector<fmwk::VertexWithNormal> vertexes{};
        std::vector<uint32_t> indices{};


        for(auto& vertex : vertexDict.getVertices()){
            vertexes.push_back({vertex.pos, vertex.UV, vertex.norm});
        }

        for(auto& triangle : vertexDict.getTriangles()){
            for(int i = 0; i < 3; i++)
                indices.push_back(triangle[i]);
        }
        return {vertexes, indices};
    }
} // fmwk