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
        explicit BossBarController(Health &bossHealth);

        void postUpdate() override;

    private:
        Health& _bossHealth;
    };

} // fmwk

#endif //DEMO_BOSSBARCONTROLLER_H
