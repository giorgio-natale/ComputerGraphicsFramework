//
// Created by Matilde on 22/08/2023.
//

#ifndef DEMO_POWERUPINCREASESPEED_H
#define DEMO_POWERUPINCREASESPEED_H

#include "PowerUp.h"

namespace fmwk {

    class PowerUpIncreaseSpeed : public PowerUp {
    public:
        void applyPowerUp() override;
    };

} // fmwk

#endif //DEMO_POWERUPINCREASESPEED_H
