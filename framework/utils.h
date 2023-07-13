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

    class EulerVector{
    public:
        EulerVector(float x, float y, float z) : EulerVector(glm::vec3(x, y, z)){}
        explicit EulerVector(glm::vec3 radiantAngles){
            for(int i = 0; i < 3; i++)
                _eulerVector[i] = radiantAngles[i] > glm::radians(180.0f) ? radiantAngles[i] - glm::radians(360.0f) : (radiantAngles[i] < glm::radians(-180.0f) ? radiantAngles[i] + glm::radians(360.0f) : radiantAngles[i]);
                //_eulerVector[i] = radiantAngles[i];
        }
        [[nodiscard]] EulerVector operator+ (glm::vec3 rotationVector) const{
            return EulerVector(_eulerVector + rotationVector);
        }
        [[nodiscard]] EulerVector operator+ (EulerVector rotationVector) const{
            return EulerVector(_eulerVector + rotationVector.getVector());
        }

        [[nodiscard]] float operator[](int index) const{
            return _eulerVector[index];
        }

        [[nodiscard]] glm::vec3 getVector() const{
            return this->_eulerVector;
        }
    private:
        glm::vec3 _eulerVector = glm::vec3(0);
    };

    /*
     * TRANSFORMATION FUNCTIONS
    */
    inline glm::mat4 rotationAroundPoint(glm::vec3 center, float yaw, float pitch){
        return glm::translate(I, center)
                * glm::rotate(I, yaw, Y)
                * glm::rotate(I, -pitch, X)
                * glm::translate(I, -1.0f * center);
    }

    inline EulerVector eulerZXYfromQuaternion(glm::quat quaternion){
        auto m = glm::mat4(quaternion);
        auto euler = glm::vec3(1);

        float m11 = m[0][0], m12 = m[0][1], m13 = m[0][2];
        float m21 = m[1][0], m22 = m[1][1], m23 = m[1][2];
        float m31 = m[2][0], m32 = m[2][1], m33 = m[2][2];

        euler.x = glm::asin(glm::clamp(m32, -1.0f, 1.0f ));

        if (glm::abs(m32) < 0.9999999f) {
            euler.y = std::atan2( -m31, m33 );
            euler.z = std::atan2( -m12, m22 );

        } else {

            euler.y = 0;
            euler.z = std::atan2( m21, m11 );
        }

        return EulerVector(euler);
    }

    inline glm::quat quaternionFromEulerZXY(EulerVector eulerVector){
        glm::quat x = glm::quat(glm::vec3(eulerVector[0], 0, 0));
        glm::quat y = glm::quat(glm::vec3(0, eulerVector[1], 0));
        glm::quat z = glm::quat(glm::vec3(0, 0, eulerVector[2]));
        return y * x * z;
    }


}


#endif //A07_UTILS_H
