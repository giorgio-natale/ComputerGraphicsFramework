//
// Created by Matilde on 22/08/2023.
//

#ifndef DEMO_POWERUPDECREASEBULLETCOOLDOWN_H
#define DEMO_POWERUPDECREASEBULLETCOOLDOWN_H

#include "PowerUp.h"

namespace fmwk {

    class PowerUpDecreaseBulletCoolDown : public PowerUp {
    public:
        void applyPowerUp() override;
    };

} // fmwk

#endif //DEMO_POWERUPDECREASEBULLETCOOLDOWN_H
