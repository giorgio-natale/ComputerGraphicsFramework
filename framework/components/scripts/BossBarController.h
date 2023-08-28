//
// Created by drgio on 22/08/2023.
//

#ifndef DEMO_BOSSBARCONTROLLER_H
#define DEMO_BOSSBARCONTROLLER_H

#include "Component.h"
#include "Health.h"

namespace fmwk {

    class BossBarController : public Component{
    public:
        explicit BossBarController(Health &bossHealth, int index);

        void postUpdate() override;

    private:
        Health& _bossHealth;
        int _index;
    };

} // fmwk

#endif //DEMO_BOSSBARCONTROLLER_H
