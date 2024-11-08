#include "FinalBossController.h"
#include "../transform/Transform.h"
#include "../../GameEngine.h"

namespace fmwk {

    FinalBossController::FinalBossController(const glm::vec3 &roomCenter, Transform &characterTransform) : Component("FinalBossController"),
                                                                                                     roomCenter(
                                                                                                             roomCenter), _characterTransform(characterTransform) {}

    void FinalBossController::update() {
        auto characterPosition = _characterTransform.getPosition();
        auto& bossTransform = _parentEntity->getTransform();
        auto bossPosition = bossTransform.getPosition();
        auto newBossPositionX = roomCenter.x + (roomCenter.x - characterPosition.x);
        auto newBossPositionZ = roomCenter.z + (roomCenter.z - characterPosition.z);
        bossTransform.setPosition(glm::vec3(newBossPositionX, bossPosition.y, newBossPositionZ));

        //make the boss look in the direction of the vector between him and the character
        bossTransform.setRotation(glm::quatLookAt(glm::normalize(bossPosition - characterPosition), Y));
    }
}