//
// Created by drgio on 22/08/2023.
//

#ifndef DEMO_BOSSBARUI_H
#define DEMO_BOSSBARUI_H

#include "Blueprint.h"

namespace fmwk {

    class BossBarUI : public Blueprint{
    public:
        explicit BossBarUI(Health &bossHealth);

    protected:
        void buildEntity() override;
    private:
        Health& _bossHealth;
    };

} // fmwk

#endif //DEMO_BOSSBARUI_H
