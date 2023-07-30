#include <string>


#ifndef A07_COMPONENT_H
#define A07_COMPONENT_H


namespace fmwk {

    class Entity;
    class Component {
    public:
        explicit Component(std::string const& name);
        [[nodiscard]] std::string getName() const;
        void setParent(Entity* parentEntity);
        Entity const* getParent();
        void removeParent();
        virtual void update(){};
        virtual void postUpdate(){};
        [[nodiscard]] virtual bool isProvisioned() const {return true;};
        virtual ~Component();
        Component (const Component&) = delete;
        Component& operator= (const Component&) = delete;
        void markForRemoval();
        [[nodiscard]] bool isMarkedForRemoval() const;



    protected:
        std::string _name;
        Entity *_parentEntity;
        Entity * getParentEntity();
        bool _toBeRemoved;
    };

} // fmwk

#endif //A07_COMPONENT_H
