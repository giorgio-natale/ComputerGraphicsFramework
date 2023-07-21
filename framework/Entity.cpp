//
// Created by drgio on 30/04/2023.
//

#include "Entity.h"
#include "Transform.h"

#include <stdexcept>
#include <algorithm>
#include <memory>

namespace fmwk {
    Entity::Entity(std::string const& name) :
            Entity(name, glm::vec3(0), EulerVector(0,0,0)){}
    Entity::Entity(const std::string &name, const glm::vec3 &initialPosition, const EulerVector &initialRotation) :
            Entity(name, initialPosition, quaternionFromEulerZXY(initialRotation)){}

    Entity::Entity(const std::string &name, const glm::vec3 &initialPosition, const glm::quat &initialRotation) {
        _name = name;
        auto transform = std::make_unique<Transform>("Transform", initialPosition, initialRotation, glm::vec3(1));
        addComponent(std::move(transform));
    }

    std::string Entity::getName() const{
        return _name;
    }

    Component& Entity::getComponentByName(std::string const& name) const{
        auto itr = _components.find(name);
        if (itr == _components.end())
            throw std::runtime_error("Could not find component with name '" + name + "' in entity '" + _name + "'");
        return *(_components.find(name)->second);
    }
    void Entity::addComponent(std::unique_ptr<Component> component) {
        if(_components.find(component->getName()) != _components.end())
            throw std::runtime_error("Could not add component with name '" + component->getName() + "' in entity '" + _name + "' because there was another component with the same name");
        component->setParent(this);

        _components.insert({component->getName(), std::move(component)});
    }

    void Entity::removeComponentByName(const std::string &name) {
        auto componentItr = _components.find(name);
        if(componentItr != _components.end()) {
            componentItr->second->removeParent();
            _components.erase(name);
        }
    }

    std::vector<Component*> Entity::getAllComponents() const{
        std::vector<Component*> components;
        std::transform(_components.cbegin(),
                              _components.cend(),
                              std::back_inserter(components),
                              [](auto & entry){
                                return entry.second.get();
                              });
        return components;
    }

    bool Entity::hasComponent(const std::string &name) const {
        auto components = getAllComponents();
        return std::any_of(components.begin(),
                           components.end(),
                           [name](Component* component){return component->getName()==name;});
    }
} // fmwk