//
// Created by drgio on 04/08/2023.
//

#ifndef DEMO_COLLIDER_H
#define DEMO_COLLIDER_H

#include "../scripts/Component.h"

namespace fmwk {

    class Collider : public Component{
    public:
        Collider(const std::string &name, float radius, std::string const& tag);
        void provision();
        [[nodiscard]] bool isProvisioned() const override;
        [[nodiscard]] float getRadius() const;

        const std::string &getTag() const;

    private:
        float _radius;
        bool _alreadyProvisioned;
        std::string _tag;
    };

} // fmwk

#endif //DEMO_COLLIDER_H
