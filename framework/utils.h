//
// Created by drgio on 01/05/2023.
//

#ifndef A07_UTILS_H
#define A07_UTILS_H

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <GLFW/glfw3.h>
#include <chrono>


namespace fmwk {
    
    /*
     * CONSTANTS
     */
    
    const glm::mat4 I = glm::mat4(1);
    const glm::vec3 X = glm::vec3(1, 0, 0);
    const glm::vec3 Y = glm::vec3(0, 1, 0);
    const glm::vec3 Z = glm::vec3(0, 0, 1);



    /*
     * ACTUAL UTILS
     */



    /*
     * TRANSFORMATION FUNCTIONS
    */
    inline glm::mat4 rotationAroundPoint(glm::vec3 center, float yaw, float pitch){
        return glm::translate(I, center)
                * glm::rotate(I, yaw, Y)
                * glm::rotate(I, -pitch, X)
                * glm::translate(I, -1.0f * center);
    }


}


#endif //A07_UTILS_H
