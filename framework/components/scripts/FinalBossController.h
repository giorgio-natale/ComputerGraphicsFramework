//
// Created by Leonardo on 17/08/2023.
//

#ifndef DEMO_FINALBOSSCONTROLLER_H
#define DEMO_FINALBOSSCONTROLLER_H

#include <glm/vec3.hpp>
#include "Component.h"
#include "../transform/Transform.h"

namespace fmwk {

    class FinalBossController : public Component {
    public:
        FinalBossController(const glm::vec3 &roomCenter, Transform &characterTransform);

        void update() override;
    private:
        glm::vec3 roomCenter;
        Transform &_characterTransform;
    };

} // fmwk

#endif //DEMO_FINALBOSSCONTROLLER_H
