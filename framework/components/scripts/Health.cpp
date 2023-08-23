//
// Created by Matilde on 15/08/2023.
//

#include "Health.h"
#include "../../GameEngine.h"
#include "Despawner.h"

namespace fmwk {
    Health::Health(float initialLifeQuantity, float gracePeriod) :
            Component("Health"),
            _totalLifeQuantity(initialLifeQuantity),
            _gracePeriod(gracePeriod),
            _currentLifeQuantity(initialLifeQuantity) {
        _isAlwaysImmune = false;
        _timeout = 0;
    }

    Health::Health(float initialLifeQuantity, float gracePeriod, float totalLifeQuantity) :
            Component("Health"),
            _totalLifeQuantity(totalLifeQuantity),
            _currentLifeQuantity(initialLifeQuantity),
            _gracePeriod(gracePeriod) {
        _isAlwaysImmune = false;
        _timeout = 0;
    }

    void Health::update() {
        GameEngine *gameEngine = GameEngine::getInstance();
        _timeout -= gameEngine->getInput().deltaTime;
        if (_timeout < 0)
            _timeout = 0;
        if (_currentLifeQuantity <= 0)
            reinterpret_cast<Despawner&>(_parentEntity->getComponentByName("Despawner")).despawn();
    }

    void Health::takeDamage(float damage) {
        if (!hasShield() && _timeout == 0) {
            _currentLifeQuantity -= damage;
            _timeout = _gracePeriod;
        }
    }

    bool Health::isInGracePeriod() const {
        return !hasShield() && _timeout > 0;
    }

    bool Health::hasShield() const {
        return _isAlwaysImmune;
    }

    void Health::setShield(bool hasShield) {
        _isAlwaysImmune = hasShield;
        if (!hasShield)
            _timeout = _gracePeriod;
    }

    float Health::getCurrentLifeQuantity() const {
        return _currentLifeQuantity;
    }

    float Health::getCurrentLifePercentage() const {
        return _currentLifeQuantity / _totalLifeQuantity;
    }

    void Health::increaseLife(float lifeQuantity) {
        _currentLifeQuantity += lifeQuantity;
    }

    float Health::getGracePeriod() const {
        return _gracePeriod;
    }

    void Health::setTimeout(float timeout) {
        _timeout = timeout;
    }

} // fmwk