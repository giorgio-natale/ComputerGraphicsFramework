//
// Created by Matilde on 22/08/2023.
//

#ifndef DEMO_POWERUP_H
#define DEMO_POWERUP_H

#include "Component.h"

namespace fmwk {

    class PowerUp: public Component {
    public:
        explicit PowerUp();
        virtual void applyPowerUp(){};
    };

} // fmwk

#endif //DEMO_POWERUP_H
