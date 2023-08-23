//
// Created by Matilde on 23/08/2023.
//

#include <iostream>
#include "BossEnemyDespawner.h"

namespace fmwk {
    void BossEnemyDespawner::despawn() {
        std::cout << "BOSS ENEMY DESPAWNER" << std::endl;
        Despawner::despawn();
    }
} // fmwk