//
// Created by drgio on 19/08/2023.
//

#ifndef DEMO_BASICENEMY_H
#define DEMO_BASICENEMY_H

#include "Blueprint.h"

namespace fmwk {

    class BasicEnemy : public Blueprint{
    public:
        explicit BasicEnemy(const std::vector<glm::vec3> &targetPoints);

    private:
        std::vector<glm::vec3> _targetPoints;
    protected:
        void buildEntity() override;
    };

} // fmwk

#endif //DEMO_BASICENEMY_H
