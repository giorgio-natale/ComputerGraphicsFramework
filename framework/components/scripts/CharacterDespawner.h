//
// Created by Matilde on 23/08/2023.
//

#ifndef DEMO_CHARACTERDESPAWNER_H
#define DEMO_CHARACTERDESPAWNER_H

#include "Despawner.h"

namespace fmwk {

    class CharacterDespawner : public Despawner {
    public:
        void despawn() override;
    };

} // fmwk

#endif //DEMO_CHARACTERDESPAWNER_H
