//
// Created by drgio on 19/08/2023.
//

#ifndef DEMO_ROUGHSPHERE_H
#define DEMO_ROUGHSPHERE_H

#include "Blueprint.h"

namespace fmwk {

    class RoughSphere : public Blueprint{
    public:
        RoughSphere(const glm::vec3 &position, const glm::quat &rotation);

    protected:
        void buildEntity() override;

        glm::vec3 _position;
        glm::quat _rotation;
    };

} // fmwk

#endif //DEMO_ROUGHSPHERE_H
