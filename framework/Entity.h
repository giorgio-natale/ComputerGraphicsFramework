#ifndef A07_ENTITY_H
#define A07_ENTITY_H

#include <string>
#include <map>
#include <unordered_set>
#include <vector>
#include <memory>
#include "components/scripts/Component.h"
#include "utils.h"
#include "components/transform/Transform.h"
#include "components/collision/Collider.h"
#include "components/scripts/Health.h"
#include "components/sprite/SpriteUtils.h"

namespace fmwk {

    class Component;
    class Entity {
    public:
        explicit Entity(std::string const& name);
        Entity(std::string const& name, glm::vec3 const& initialPosition, glm::quat const& initialRotation);
        Entity(std::string const& name, glm::vec2 preferredSize, const std::vector<UiAnchor>& anchors, bool stretchable);

        [[nodiscard]] std::string getName() const;
        [[nodiscard]] Component& getComponentByName(std::string const& name) const;
        [[nodiscard]] Transform& getTransform() const{
            return dynamic_cast<Transform&>(getComponentByName("Transform"));
        }
        [[nodiscard]] Collider& getCollider() const{
            return dynamic_cast<Collider&>(getComponentByName("Collider"));
        }
        [[nodiscard]] Health& getHealth() const{
            return dynamic_cast<Health&>(getComponentByName("Health"));
        }
        void addComponent(std::unique_ptr<Component> component);
        void enqueueComponent(std::unique_ptr<Component> component);
        void removeComponentByName(std::string const& name);
        void enqueueComponentRemoval(std::string const& name) const;
        void flushEnqueuedComponents();
        void setPreferredRenderOrder(int preferredRenderOrder);
        [[nodiscard]] int getPreferredRenderOrder() const;

        std::vector<Component*> getAllEnqueuedComponents();

        void markForRemoval();
        [[nodiscard]] bool isMarkedForRemoval() const;
        [[nodiscard]] std::vector<Component*> getAllComponents() const;
        [[nodiscard]] bool hasComponent(std::string const& name) const;
        void setVisible(bool visible);
        [[nodiscard]] bool isVisible() const;

        Entity (const Entity&) = delete;
        Entity& operator= (const Entity&) = delete;
    private:
        std::string _name;
        std::map<std::string, std::unique_ptr<Component>> _components;
        std::map<std::string, std::unique_ptr<Component>> _enqueuedComponents;
        bool _toBeRemoved;
        int _preferredRenderOrder;
        bool _visible;

        //utils
        void addComponentToContainer(std::unique_ptr<Component> component, std::map<std::string, std::unique_ptr<Component>>& container);


    };

} // fmwk

#endif //A07_ENTITY_H
