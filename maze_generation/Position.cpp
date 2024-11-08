//
// Created by drgio on 07/08/2023.
//

#include <vector>
#include <cmath>
#include "Position.h"

namespace mgen {

    Position scalar(Direction const& dir, int scalar){
        std::vector<int> res;
        for(auto i : dir.getDirection())
            res.push_back(i * scalar);
        return {res[0], res[1], res[2]};
    }
    glm::vec3 toGlm(Position position, float unit){
        glm::vec3 res = {(float)position[0], (float)position[2], -(float)position[1]};
        return res * unit;
    }

    Position fromGlm(glm::vec3 position, float unit){
        return {(int)(std::floor(position.x / unit)), (int)(std::floor(-position.z / unit)), (int)(std::floor(position.y / unit))};
    }
} // mgen