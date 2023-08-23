//
// Created by Matilde on 23/08/2023.
//

#ifndef DEMO_DESPAWNER_H
#define DEMO_DESPAWNER_H

#include "Component.h"

namespace fmwk {

    class Despawner: public Component {
    public:
        explicit Despawner() : Component("Despawner") {}

        virtual void despawn(){};
    };

} // fmwk

#endif //DEMO_DESPAWNER_H
