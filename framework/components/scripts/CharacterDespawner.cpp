//
// Created by Matilde on 23/08/2023.
//

#include <iostream>
#include "CharacterDespawner.h"

namespace fmwk {
    void CharacterDespawner::despawn() {
        std::cout << "CHARACTER DESPAWNER" << std::endl;
        Despawner::despawn();
    }
} // fmwk