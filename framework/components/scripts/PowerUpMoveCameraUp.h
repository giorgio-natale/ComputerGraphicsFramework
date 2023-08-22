//
// Created by Matilde on 22/08/2023.
//

#ifndef DEMO_POWERUPMOVECAMERAUP_H
#define DEMO_POWERUPMOVECAMERAUP_H

#include "PowerUp.h"

namespace fmwk {

    class PowerUpMoveCameraUp : public PowerUp {
    public:
        PowerUpMoveCameraUp();

        void applyPowerUp() override;
    };

} // fmwk

#endif //DEMO_POWERUPMOVECAMERAUP_H
