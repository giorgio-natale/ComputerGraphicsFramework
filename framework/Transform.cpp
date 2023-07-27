//
// Created by drgio on 01/05/2023.
//

#include "Transform.h"

#include <utility>
#include "utils.h"

namespace fmwk {

    Transform::Transform(const std::string &name, const glm::vec3 &position, const EulerVector &eulerVector,
                         const glm::vec3 &scale) : Component(name), _position(position), _eulerVector(eulerVector),
                                                   _scale(scale) {
        _areMatricesCoherent = false;
        _quaternion = quaternionFromEulerZXY(eulerVector);
        _worldMatrix = getWorldMatrix();
        _inverseWorldMatrix = getInverseWorldMatrix();
        _alreadyProvisioned = false;
        _descriptorSet = nullptr;
    }

    Transform::Transform(const std::string &name, const glm::vec3 &position, const glm::quat &quaternion,
                         const glm::vec3 &scale): Transform(name, position, eulerZXYfromQuaternion(quaternion), scale){
    }

    glm::mat4 Transform::getWorldMatrix() const{
        if(!_areMatricesCoherent){
            computeMatrices();
        }
        return _worldMatrix;
    }
    glm::mat4 Transform::getInverseWorldMatrix() const{
        if(!_areMatricesCoherent) {
            computeMatrices();
        }
        return _inverseWorldMatrix;
    }

    void Transform::setPosition(glm::vec3 const& position) {
        _areMatricesCoherent = false;
        _position = position;
    }

    void Transform::setRotation(glm::vec3 const& rotation) {
        _areMatricesCoherent = false;
        _eulerVector = EulerVector(rotation);

        _quaternion = quaternionFromEulerZXY(_eulerVector);

    }

    void Transform::setRotation(const glm::quat &rotation) {
        _areMatricesCoherent = false;
        _eulerVector = eulerZXYfromQuaternion(rotation);
        _quaternion = rotation;
    }

    void Transform::setScale(glm::vec3 const& scale) {
        _areMatricesCoherent = false;
        _scale = scale;
    }

    void Transform::translate(glm::vec3 const& deltaPos) {
        _areMatricesCoherent = false;
        _position += deltaPos;
    }

    void Transform::rotate(glm::vec3 const& deltaAngles) {
        _areMatricesCoherent = false;
        _eulerVector = _eulerVector + deltaAngles;
        glm::quat x = glm::quat(_eulerVector.getVector()[0], X);
        glm::quat y = glm::quat(_eulerVector.getVector()[1], Y);
        glm::quat z = glm::quat(_eulerVector.getVector()[2], Z);
    }

    void Transform::rotate(const glm::quat &deltaQuaternion) {
        _areMatricesCoherent = false;
        _quaternion = deltaQuaternion * _quaternion;
        setRotation(_quaternion);
    }

    glm::mat3 Transform::getLocalDirections() const {
        auto directions = glm::mat3();
        auto worldMatrix = getWorldMatrix();
        for(int i = 0; i < 3; i++){
            directions[i] = glm::normalize(glm::vec3(worldMatrix[i]));
        }

        return directions;
    }

    void Transform::computeMatrices() const {
        auto scaleMatrix = glm::scale(I, this->_scale);
        auto rotationMatrix = glm::mat4(_quaternion);
        auto translationMatrix = glm::translate(I, this->_position);
        _worldMatrix = translationMatrix  * rotationMatrix * scaleMatrix;

        auto invScaleMatrix = glm::scale(I, glm::vec3(1/_scale[0], 1/_scale[1], 1/_scale[2]));
        auto invRotationMatrix = glm::mat4(glm::conjugate(_quaternion));
        auto invTranslationMatrix = glm::translate(I, -1.0f * this->_position);
        _inverseWorldMatrix = invScaleMatrix * invRotationMatrix * invTranslationMatrix;
        _areMatricesCoherent = true;
    }

    void Transform::provision(DescriptorSet* descriptorSet) {
        _descriptorSet = descriptorSet;
        _alreadyProvisioned = true;
    }

    void Transform::updateDescriptorSet(int currentImage) {
        EntityTransformUniformBlock ubo{};
        ubo.mMat = getWorldMatrix();
        _descriptorSet->map(currentImage, &ubo, sizeof(ubo), 0);
    }

    DescriptorSet &Transform::getDescriptorSet() {
        return *_descriptorSet;
    }

    bool Transform::isProvisioned() const {
        return _alreadyProvisioned;
    }


} // fmwk