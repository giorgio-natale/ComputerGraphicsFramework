//
// Created by Matilde on 23/08/2023.
//

#include <iostream>
#include "DefaultDespawner.h"
#include "../../Entity.h"

namespace fmwk {
    void DefaultDespawner::despawn() {
        _parentEntity->markForRemoval();
    }
} // fmwk