//
// Created by Matilde on 07/08/2023.
//

#ifndef DEMO_TESTCOLLISIONCHECKER_H
#define DEMO_TESTCOLLISIONCHECKER_H

#include "Component.h"

namespace fmwk {

    class TestCollisionChecker: public Component {
    public:
        explicit TestCollisionChecker(const std::string &name);

        void update() override;
    };

} // fmwk

#endif //DEMO_TESTCOLLISIONCHECKER_H
