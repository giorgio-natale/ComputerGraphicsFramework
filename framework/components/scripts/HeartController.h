//
// Created by drgio on 21/08/2023.
//

#ifndef DEMO_HEARTCONTROLLER_H
#define DEMO_HEARTCONTROLLER_H

#include "Component.h"

namespace fmwk {

    class HeartController : public Component{
    public:
        void postUpdate() override;

        explicit HeartController(int index);

    private:
        int _index;
    };

} // fmwk

#endif //DEMO_HEARTCONTROLLER_H
