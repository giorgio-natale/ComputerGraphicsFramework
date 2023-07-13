#ifndef A07_ENTITY_H
#define A07_ENTITY_H

#include <string>
#include <map>
#include <unordered_set>
#include <vector>
#include <memory>
#include "Component.h"
#include "utils.h"
#include "Transform.h"

namespace fmwk {

    class Component;
    class Entity {
    public:
        explicit Entity(std::string const& name);
        Entity(std::string const& name, glm::vec3 const& initialPosition, EulerVector const& initialRotation);
        Entity(std::string const& name, glm::vec3 const& initialPosition, glm::quat const& initialRotation);

        [[nodiscard]] std::string getName() const;
        [[nodiscard]] Component& getComponentByName(std::string const& name) const;
        [[nodiscard]] Transform& getTransform() const{
            return dynamic_cast<Transform&>(getComponentByName("Transform"));
        }
        void addComponent(std::unique_ptr<Component> component);
        void removeComponentByName(std::string const& name);
        [[nodiscard]] std::vector<Component*> getAllComponents() const;

        Entity (const Entity&) = delete;
        Entity& operator= (const Entity&) = delete;
    private:
        std::string _name;
        std::map<std::string, std::unique_ptr<Component>> _components;

    };

} // fmwk

#endif //A07_ENTITY_H
