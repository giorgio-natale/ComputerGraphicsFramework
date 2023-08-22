//
// Created by Matilde on 22/08/2023.
//

#include "FollowEntity.h"
#include "../../Entity.h"

namespace fmwk {
    FollowEntity::FollowEntity(Transform &targetTransform) : Component("FollowEntity"), _targetTransform(
            targetTransform) {}

    void FollowEntity::postUpdate() {
        _parentEntity->getTransform().setPosition(_targetTransform.getPosition());
    }
} // fmwk