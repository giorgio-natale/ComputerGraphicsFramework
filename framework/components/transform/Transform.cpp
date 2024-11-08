//
// Created by drgio on 01/05/2023.
//

#include "Transform.h"

#include <utility>
#include "../../utils.h"
#include "../../Entity.h"
#include "../mesh/MeshComponent.h"

namespace fmwk {

    Transform::Transform(const glm::vec3 &position, const glm::quat &quaternion,
                         const glm::vec3 &scale): Component("Transform"), _position(position), _scale(scale){
        _quaternion = quaternion;
        _worldMatrix = getWorldMatrix();
        _inverseWorldMatrix = getInverseWorldMatrix();
        _alreadyProvisioned = false;
        _descriptorSet = nullptr;
        _areMatricesCoherent = false;
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


    void Transform::setRotation(const glm::quat &rotation) {
        _areMatricesCoherent = false;
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
        glm::mat4 modelMatrix = I;
        if(_parentEntity->hasComponent("Mesh")){
            modelMatrix = dynamic_cast<MeshComponent&>(_parentEntity->getComponentByName("Mesh")).getModel().getModelTransform().getWorldMatrix();
        }
        ubo.mMat = getWorldMatrix() * modelMatrix;
        ubo.nMat = glm::transpose(glm::inverse(glm::mat3(ubo.mMat)));
        _descriptorSet->map(currentImage, &ubo, sizeof(ubo), 0);
    }

    DescriptorSet &Transform::getDescriptorSet() {
        return *_descriptorSet;
    }

    bool Transform::isProvisioned() const {
        return _alreadyProvisioned;
    }


} // fmwk