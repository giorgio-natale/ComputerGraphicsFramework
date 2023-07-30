//
// Created by drgio on 29/04/2023.
//

#include "Component.h"
#include "../../Entity.h"

#include <utility>

namespace fmwk {

    std::string Component::getName() const{
        return this->_name;
    }

    Component::Component(std::string const& name) {
        _name = name;
        _parentEntity = nullptr;
        _toBeRemoved = false;
    }

    Component::~Component() = default;

    void Component::setParent(Entity *parentEntity) {
        _parentEntity = parentEntity;
    }

    void Component::removeParent() {
        _parentEntity = nullptr;
    }

    Entity * Component::getParentEntity() {
        return _parentEntity;
    }

    Entity const*Component::getParent() {
        return _parentEntity;
    }

    void Component::markForRemoval() {
        _toBeRemoved = true;
    }

    bool Component::isMarkedForRemoval() const {
        return _toBeRemoved;
    }

} // fmwk