//
// Created by drgio on 07/08/2023.
//

#include "VertexDictionary.h"

namespace mgen {
    void VertexDictionary::addRectangle(Rectangle r){
        std::array<int, 4> indexes{};
        for(int i = 0; i < r.size(); i++){
            auto tmpArrayVec = std::array<float, 8>{r[i].pos.x, r[i].pos.y, r[i].pos.z, r[i].UV.x, r[i].UV.y, r[i].norm.x, r[i].norm.y, r[i].norm.z};
            if(_dict.find(tmpArrayVec) == _dict.end()){
                _dict.insert({tmpArrayVec, _nextId});
                _vertices.push_back(r[i]);
                _nextId++;
            }
            indexes[i] = _dict.find(tmpArrayVec)->second;
        }

        _triangles.push_back(IndexedTriangle{indexes[0], indexes[1], indexes[2]});
        _triangles.push_back(IndexedTriangle{indexes[2], indexes[3], indexes[0]});
    }

    std::vector<MazeVertex> VertexDictionary::getVertices(){
        return _vertices;
    }
    std::vector<IndexedTriangle> VertexDictionary::getTriangles(){
        return _triangles;
    }
} // mgen