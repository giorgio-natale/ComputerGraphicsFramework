//
// Created by drgio on 01/08/2023.
//

#ifndef DEMO_BLUEPRINT_H
#define DEMO_BLUEPRINT_H

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <string>
#include "../Entity.h"

namespace fmwk {

    class Blueprint {
    public:
        void spawnInstance();
        void addInstance();
    protected:
        virtual void buildEntity() = 0;
        virtual Entity* getMainEntity();
        static int getNewNumber();
    private:
        static int _uniqueNumber;
    };

} // fmwk

#endif //DEMO_BLUEPRINT_H
