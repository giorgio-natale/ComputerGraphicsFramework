//
// Created by drgio on 07/08/2023.
//

#include <algorithm>
#include <glm/geometric.hpp>
#include "FragmentedFace.h"
#include "../framework/utils.h"

namespace mgen {

    std::vector<Rectangle> FragmentedFace::buildRectangles() const{
        auto rectangles = std::vector<Rectangle>();
        int i = 0;
        while(i < _facesIncluded.size()){
            //find first non-empty face
            while(i < _facesIncluded.size() && !_facesIncluded[i]){i++;}
            if(i >= _facesIncluded.size())
                break;

            //find last non-empty face of the fragment
            int j = i+1;
            while(j < _facesIncluded.size() && _facesIncluded[j]){j++;}
            j--;

            int fragmentLength = j-i+1;
            Position startFragmentPosition = Position(_startPosition + scalar(_growingDirection, i));

            if(fragmentLength > 0)
                rectangles.push_back(buildRectangle(startFragmentPosition, fragmentLength));
            i = j+1;

        }

        return rectangles;
    }

    void FragmentedFace::deleteFace(int faceIndex){
        if(faceIndex < 0 || faceIndex >= _length){
            std::ostringstream oss;
            oss << "Cannot delete face index " << faceIndex << " because it is out of bound (" << _length - 1 << ")";
            throw std::runtime_error(oss.str());
        }

        _facesIncluded[faceIndex] = false;
    }

    Rectangle FragmentedFace::buildRectangle(Position fragmentStartPosition, int fragmentLength) const {
        glm::vec3 startPoint = toGlm(fragmentStartPosition, _unit) + glm::vec3(0.5f, 0.5f, -0.5f) * _unit;
        glm::vec3 moveToEdge = toGlm(_side, _unit) * 0.5f;
        glm::vec3 moveAwayFromGrowth = -1.0f * toGlm(_growingDirection, _unit) * 0.5f;

        Direction const& finalDirection = _side.getPerpendicularTo(_growingDirection);
        glm::vec3 finalMovement = toGlm(finalDirection, _unit) * 0.5f;

        glm::vec3 point1 = startPoint + moveToEdge + moveAwayFromGrowth + finalMovement;
        glm::vec3 point2 = startPoint + moveToEdge + moveAwayFromGrowth - finalMovement;
        glm::vec3 point3 = point1 + toGlm(_growingDirection, _unit) * (float)fragmentLength;
        glm::vec3 point4 = point2 + toGlm(_growingDirection, _unit) * (float)fragmentLength;

        std::vector<glm::vec3> points = {point1, point2, point3, point4};

        MazeVertex vertex00{}, vertex10{}, vertex11{}, vertex01{};
        glm::vec4 tangent;
        if(_side == Direction::SOUTH){
            tangent = glm::vec4(fmwk::X, 1);
            std::sort(points.begin(), points.end(), [](const auto& p1, const auto& p2){
                return p1.y < p2.y || (p1.y == p2.y) && p1.x < p2.x;
            });
        }else if(_side == Direction::EAST){
            tangent = glm::vec4(-fmwk::Z, 1);
            std::sort(points.begin(), points.end(), [](const auto& p1, const auto& p2){
                return p1.y < p2.y || (p1.y == p2.y) && p1.z > p2.z;
            });
        }else if(_side == Direction::WEST){
            tangent = glm::vec4(fmwk::Z, 1);
            std::sort(points.begin(), points.end(), [](const auto& p1, const auto& p2){
                return p1.y < p2.y || (p1.y == p2.y) && p1.z < p2.z;
            });
        }else if(_side == Direction::NORTH){
            tangent = glm::vec4(-fmwk::X, 1);
            std::sort(points.begin(), points.end(), [](const auto& p1, const auto& p2){
                return p1.y < p2.y || (p1.y == p2.y) && p1.x > p2.x;
            });
        }else if(_side == Direction::TOP){
            tangent = glm::vec4(fmwk::X, 1);

            std::sort(points.begin(), points.end(), [](const auto& p1, const auto& p2){
                return p1.z > p2.z || (p1.z == p2.z) && p1.x < p2.x;
            });
        }else{
            throw std::runtime_error("Cannot create vertexes for bottom faces");
        }

        glm::vec3 normal = glm::normalize(toGlm(_side, 1.0f));

        vertex00.pos = points[0];
        vertex00.UV = {0,glm::distance(points[0], points[2]) / _unit};
        vertex00.norm = normal;
        //vertex00.tan = tangent;

        vertex10.pos = points[1];
        vertex10.UV = {glm::distance(points[1], points[0]) / _unit ,glm::distance(points[3], points[1]) / _unit};
        vertex10.norm = normal;
        //vertex10.tan = tangent;

        vertex11.pos = points[3];
        vertex11.UV = {glm::distance(points[2], points[3]) / _unit,0};
        vertex11.norm = normal;
        //vertex11.tan = tangent;

        vertex01.pos = points[2];
        vertex01.UV = {0,0};
        vertex01.norm = normal;
        //vertex01.tan = tangent;


        return {vertex00, vertex10, vertex11, vertex01};
    }
} // mgen