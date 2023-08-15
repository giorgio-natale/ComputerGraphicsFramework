//
// Created by Matilde on 15/08/2023.
//

#ifndef DEMO_HEALTH_H
#define DEMO_HEALTH_H

#include "Component.h"

namespace fmwk {

    class Health : public Component {
    public:
        Health(float lifeQuantity, float gracePeriod);

        void update() override;

        void takeDamage(float damage);

        [[nodiscard]] bool isInGracePeriod() const;

        [[nodiscard]] bool hasShield() const;

        void setShield(bool hasShield);

        [[nodiscard]] float getCurrentLifeQuantity() const;

        [[nodiscard]] float getCurrentLifePercentage() const;

    private:
        const float _initialLifeQuantity;
        float _currentLifeQuantity;
        float _timeout;
        bool _isAlwaysImmune;
        const float _gracePeriod;

    };

} // fmwk

#endif //DEMO_HEALTH_H
