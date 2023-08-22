//
// Created by Matilde on 22/08/2023.
//

#ifndef DEMO_TRIGGERPOWERUP_H
#define DEMO_TRIGGERPOWERUP_H

#include "Component.h"

namespace fmwk {

    class TriggerPowerUp: public Component {
    public:
        explicit TriggerPowerUp();

        void update() override;

    };

} // fmwk

#endif //DEMO_TRIGGERPOWERUP_H
