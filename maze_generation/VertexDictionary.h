//
// Created by drgio on 07/08/2023.
//

#ifndef DEMO_VERTEXDICTIONARY_H
#define DEMO_VERTEXDICTIONARY_H

#include <vector>
#include <map>
#include "mazeUtils.h"

namespace mgen {

    class VertexDictionary{
    public:
        void addRectangle(Rectangle r);
        std::vector<MazeVertex> getVertices();
        std::vector<IndexedTriangle> getTriangles();
    private:
        int _nextId = 0;
        std::map<std::array<float, 8>, int> _dict;
        std::vector<MazeVertex> _vertices;
        std::vector<IndexedTriangle> _triangles;
    };

} // mgen

#endif //DEMO_VERTEXDICTIONARY_H
