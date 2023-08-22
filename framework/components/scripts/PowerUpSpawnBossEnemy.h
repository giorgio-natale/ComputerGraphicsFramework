//
// Created by Matilde on 22/08/2023.
//

#ifndef DEMO_POWERUPSPAWNBOSSENEMY_H
#define DEMO_POWERUPSPAWNBOSSENEMY_H

#include "PowerUp.h"

namespace fmwk {

    class PowerUpSpawnBossEnemy : public PowerUp {
    public:
        PowerUpSpawnBossEnemy();

        void applyPowerUp() override;
    };

} // fmwk

#endif //DEMO_POWERUPSPAWNBOSSENEMY_H
