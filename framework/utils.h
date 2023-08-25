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

    inline glm::quat createQuat(glm::vec3 direction, float angleDegrees){
        return glm::rotate(glm::quat(1,0,0,0), glm::radians(angleDegrees), glm::normalize(direction));
    }

    inline glm::quat createQuat(glm::vec3 direction1, float angleDegrees1, glm::vec3 direction2, float angleDegrees2){
        return glm::rotate(glm::rotate(glm::quat(1,0,0,0), glm::radians(angleDegrees1), glm::normalize(direction1)), glm::radians(angleDegrees2), glm::normalize(direction2));
    }

    inline glm::quat createQuat(glm::vec3 direction1, float angleDegrees1, glm::vec3 direction2, float angleDegrees2, glm::vec3 direction3, float angleDegrees3){
        return glm::rotate(glm::rotate(glm::rotate(glm::quat(1,0,0,0), glm::radians(angleDegrees1), glm::normalize(direction1)), glm::radians(angleDegrees2), glm::normalize(direction2)), glm::radians(angleDegrees3), glm::normalize(direction3));
    }


}


#endif //A07_UTILS_H
