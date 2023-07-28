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
            Entity(name, glm::vec3(0), glm::quat()){}

    Entity::Entity(const std::string &name, const glm::vec3 &initialPosition, const glm::quat &initialRotation) {
        _name = name;
        auto transform = std::make_unique<Transform>("Transform", initialPosition, initialRotation, glm::vec3(1));
        addComponent(std::move(transform));
        _toBeRemoved = false;
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

    void Entity::addComponentToContainer(std::unique_ptr<Component> component,
                                         std::map<std::string, std::unique_ptr<Component>> &container) {
        if(container.find(component->getName()) != container.end())
            throw std::runtime_error("Could not add component with name '" + component->getName() + "' in entity '" + _name + "' because there was another component with the same name");
        component->setParent(this);

        container.insert({component->getName(), std::move(component)});
    }

    void Entity::addComponent(std::unique_ptr<Component> component) {
        addComponentToContainer(std::move(component), _components);
    }

    void Entity::enqueueComponent(std::unique_ptr<Component> component) {
        Entity::addComponentToContainer(std::move(component), _enqueuedComponents);
    }

    void Entity::enqueueComponentRemoval(const std::string &name) const {
        //TODO: decide if making the application crash if the component is not found or do nothing (in this case it crashes)
        auto& component = getComponentByName(name);
        component.markForRemoval();
    }

    void Entity::flushEnqueuedComponents() {
        for(Component* component : getAllComponents())
            if(component->isMarkedForRemoval())
                removeComponentByName(component->getName());

        for(auto& [key, component] : _enqueuedComponents){
            addComponent(std::move(component));
        }

        _enqueuedComponents.clear();

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

    void Entity::markForRemoval() {
        _toBeRemoved = true;
    }

    bool Entity::isMarkedForRemoval() const {
        return _toBeRemoved;
    }

    std::vector<Component *> Entity::getAllEnqueuedComponents() {
        std::vector<Component*> components;
        std::transform(_enqueuedComponents.cbegin(),
                       _enqueuedComponents.cend(),
                       std::back_inserter(components),
                       [](auto & entry){
                           return entry.second.get();
                       });
        return components;
    }


} // fmwk