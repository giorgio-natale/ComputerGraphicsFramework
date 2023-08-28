//
// Created by drgio on 22/08/2023.
//

#include "BossBarController.h"
#include "../../Entity.h"
#include "../texture/TextureComponent.h"
#include "../../GameEngine.h"
#include "Despawner.h"

namespace fmwk {
    BossBarController::BossBarController(Health &bossHealth, int index) : Component("BossHealthController"),
                                                               _bossHealth(bossHealth),
                                                               _index(index){}

    void BossBarController::postUpdate() {
        if(_bossHealth.getCurrentLifeQuantity() > 0) {
            int healthLevel = (int) (_bossHealth.getCurrentLifeQuantity() / 10.0f);
            _parentEntity->setVisible(healthLevel == _index);
        }else{
            _parentEntity->markForRemoval();
        }
    }
} // fmwk