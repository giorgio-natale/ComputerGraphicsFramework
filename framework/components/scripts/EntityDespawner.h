//
// Created by drgio on 04/08/2023.
//

#ifndef DEMO_ENTITYDESPAWNER_H
#define DEMO_ENTITYDESPAWNER_H

#include "Component.h"

namespace fmwk {

    class EntityDespawner : public Component{
    public:
        explicit EntityDespawner(const std::string &name, float delay);

        void update() override;
    private:
        float _delay;
        float _timeElapsed;
    };

} // fmwk

#endif //DEMO_ENTITYDESPAWNER_H
