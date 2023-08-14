//
// Created by drgio on 04/08/2023.
//

#ifndef DEMO_COLLIDER_H
#define DEMO_COLLIDER_H

#include <glm/vec3.hpp>
#include "../scripts/Component.h"

namespace fmwk {

    class Collider : public Component{
    public:
        Collider(float radius, std::string tag, glm::vec3 centerOffset);
        void provision();
        [[nodiscard]] bool isProvisioned() const override;
        [[nodiscard]] float getRadius() const;

        [[nodiscard]] const glm::vec3 &getCenterOffset() const;

        [[nodiscard]] const std::string &getTag() const;

    private:
        float _radius;
        bool _alreadyProvisioned;
        glm::vec3 _centerOffset;
        std::string _tag;
    };

} // fmwk

#endif //DEMO_COLLIDER_H
