//
// Created by Matilde on 22/08/2023.
//

#ifndef DEMO_SHIELD_H
#define DEMO_SHIELD_H

#include "Blueprint.h"

namespace fmwk {

    class Shield: public Blueprint{
    protected:
        void buildEntity() override;
    };

} // fmwk

#endif //DEMO_SHIELD_H
