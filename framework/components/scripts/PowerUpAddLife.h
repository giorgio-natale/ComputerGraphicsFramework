//
// Created by Matilde on 22/08/2023.
//

#ifndef DEMO_POWERUPADDLIFE_H
#define DEMO_POWERUPADDLIFE_H

#include "PowerUp.h"

namespace fmwk {

    class PowerUpAddLife : public PowerUp {
    public:
        PowerUpAddLife();

        void applyPowerUp() override;

    };

} // fmwk

#endif //DEMO_POWERUPADDLIFE_H
