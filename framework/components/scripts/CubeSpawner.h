//
// Created by drgio on 24/07/2023.
//

#ifndef DEMO_CUBESPAWNER_H
#define DEMO_CUBESPAWNER_H

#include "Component.h"

namespace fmwk {

    class CubeSpawner : public Component{
    public:
        explicit CubeSpawner(const std::string &name);
        void update() override;
    private:
        bool _spawnEnabled = true;
    };

} // fmwk

#endif //DEMO_CUBESPAWNER_H
