//
// Created by Matilde on 23/08/2023.
//

#ifndef DEMO_SHIELDDESPAWNER_H
#define DEMO_SHIELDDESPAWNER_H

#include "Despawner.h"

namespace fmwk {

    class ShieldDespawner : public Despawner {
    public:
        void despawn() override;
    };

} // fmwk

#endif //DEMO_SHIELDDESPAWNER_H
