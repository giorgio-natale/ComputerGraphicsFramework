//
// Created by Matilde on 22/08/2023.
//

#ifndef DEMO_FOLLOWENTITY_H
#define DEMO_FOLLOWENTITY_H

#include "Component.h"
#include "../transform/Transform.h"

namespace fmwk {

    class FollowEntity : public Component {
    public:
        void postUpdate() override;

        explicit FollowEntity(Transform &targetTransform);

    private:
        Transform &_targetTransform;

    };

} // fmwk

#endif //DEMO_FOLLOWENTITY_H
