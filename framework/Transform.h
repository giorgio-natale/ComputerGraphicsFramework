//
// Created by drgio on 01/05/2023.
//

#ifndef A07_TRANSFORM_H
#define A07_TRANSFORM_H

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Component.h"
#include "utils.h"

namespace fmwk {
    class Transform : public Component{
    public:
        Transform(const std::string &name, const glm::vec3 &position, const EulerVector &eulerVector,
                  const glm::vec3 &scale);
        Transform(const std::string &name, const glm::vec3 &position, const glm::quat &quaternion,
                  const glm::vec3 &scale);

        glm::mat4 getWorldMatrix() const;
        glm::mat4 getInverseWorldMatrix() const;

        glm::vec3 getPosition() const {return _position;}
        glm::vec3 getRotation() const {return _eulerVector.getVector();}
        glm::vec3 getScale() const {return _scale;}

        glm::mat3 getLocalDirections() const;

        void setPosition(glm::vec3 const& position);
        void setRotation(glm::vec3 const& rotation);
        void setRotation(glm::quat const& rotation);
        void setScale(glm::vec3 const& scale);

        void translate(glm::vec3 const& deltaPos);
        void rotate(glm::vec3 const& deltaAngles);
        void rotate(glm::quat const& deltaQuaternion);

    private:
        glm::vec3 _position;
        EulerVector _eulerVector;
        glm::quat _quaternion = glm::quat(1,0,0,0);
        glm::vec3 _scale;
        mutable bool _areMatricesCoherent;
        mutable glm::mat4 _worldMatrix = I;
        mutable glm::mat4 _inverseWorldMatrix = I;

        void computeMatrices() const;
    };

} // fmwk

#endif //A07_TRANSFORM_H
