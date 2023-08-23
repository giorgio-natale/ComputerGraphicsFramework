//
// Created by Matilde on 23/08/2023.
//

#ifndef DEMO_DEFAULTDESPAWNER_H
#define DEMO_DEFAULTDESPAWNER_H

#include "Despawner.h"

namespace fmwk {

    class DefaultDespawner : public Despawner {
    public:
        void despawn() override;
    };

} // fmwk

#endif //DEMO_DEFAULTDESPAWNER_H
