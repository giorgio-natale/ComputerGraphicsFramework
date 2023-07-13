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
        void removeParent();
        virtual void update(){};
        virtual void postUpdate(){};
        virtual ~Component();
        Component (const Component&) = delete;
        Component& operator= (const Component&) = delete;


    protected:
        std::string _name;
        Entity const* _parentEntity;
        [[nodiscard]] const Entity * getParentEntity() const;
    };

} // fmwk

#endif //A07_COMPONENT_H
