//
// Created by drgio on 07/08/2023.
//

#ifndef DEMO_MAZEUTILS_H
#define DEMO_MAZEUTILS_H

#include <array>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

namespace mgen{
    struct MazeVertex{
        glm::vec3 pos;
        glm::vec2 UV;
        glm::vec3 norm;
    };
    typedef std::array<MazeVertex, 4> Rectangle;
    typedef std::array<int, 3> IndexedTriangle;
}

#endif //DEMO_MAZEUTILS_H
