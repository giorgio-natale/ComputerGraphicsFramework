//
// Created by Matilde on 22/08/2023.
//

#ifndef DEMO_POWERUPCUBE_H
#define DEMO_POWERUPCUBE_H

#include "Blueprint.h"

namespace fmwk {

    enum PowerUpType {
        SET_SHIELD, INCREASE_SPEED, DECREASE_BULLET_COOL_DOWN, ADD_LIFE, SPAWN_BOSS_ENEMY
    };

    class PowerUpCube : public Blueprint {

    public:
        PowerUpCube(const glm::vec3 &position, PowerUpType type);

    protected:
        void buildEntity() override;

    private:
        glm::vec3 _position;
        PowerUpType _type;
    };

} // fmwk

#endif //DEMO_POWERUPCUBE_H
