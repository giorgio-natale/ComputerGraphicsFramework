//
// Created by drgio on 09/08/2023.
//

#ifndef DEMO_ENEMYCOLLISIONBEHAVIOUR_H
#define DEMO_ENEMYCOLLISIONBEHAVIOUR_H

#include "Component.h"

namespace fmwk {

    class EnemyCollisionBehaviour : public Component{
    public:
        explicit EnemyCollisionBehaviour();

        void update() override;
    };

} // fmwk

#endif //DEMO_ENEMYCOLLISIONBEHAVIOUR_H
