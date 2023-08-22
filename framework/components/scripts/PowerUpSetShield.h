//
// Created by Matilde on 22/08/2023.
//

#ifndef DEMO_POWERUPSETSHIELD_H
#define DEMO_POWERUPSETSHIELD_H

#include "Component.h"
#include "PowerUp.h"

namespace fmwk {

    class PowerUpSetShield : public PowerUp {
    public:
        void applyPowerUp() override;
    };

} // fmwk

#endif //DEMO_POWERUPSETSHIELD_H
