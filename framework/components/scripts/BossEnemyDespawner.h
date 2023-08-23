//
// Created by Matilde on 23/08/2023.
//

#ifndef DEMO_BOSSENEMYDESPAWNER_H
#define DEMO_BOSSENEMYDESPAWNER_H

#include "Despawner.h"

namespace fmwk {

    class BossEnemyDespawner : public Despawner {
    public:
        void despawn() override;
    };

} // fmwk

#endif //DEMO_BOSSENEMYDESPAWNER_H
