//
// Created by drgio on 21/08/2023.
//

#ifndef DEMO_BOSSENEMY_H
#define DEMO_BOSSENEMY_H

#include "Blueprint.h"

namespace fmwk {

    class BossEnemy : public Blueprint{
    public:
        explicit BossEnemy(Transform &characterTransform);

    protected:
        void buildEntity() override;
    private:
        Transform& _characterTransform;
    };

} // fmwk

#endif //DEMO_BOSSENEMY_H
