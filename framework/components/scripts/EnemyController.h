//
// Created by Leonardo on 08/08/2023.
//

#ifndef DEMO_ENEMYCONTROLLER_H
#define DEMO_ENEMYCONTROLLER_H

#include <vector>
#include <glm/vec3.hpp>
#include "Component.h"

namespace fmwk {

    class EnemyController : public Component{
    public :
        EnemyController(const std::vector<glm::vec3> &targetPositions, float speed,
                        float laneWidth, float meshRadius);

        void update() override;

    private :
        std::vector<glm::vec3> _targetPositions;
        int _currentTargetIndex = 0;
        float _speed;
        float _laneWidth;
        float _meshRadius;

        static void processVector(std::vector<glm::vec3> &targetPositions);

        static void findZPath(std::vector<glm::vec3> &targetPositions, float laneWidth, float meshRadius);
    };

} // fmwk

#endif //DEMO_ENEMYCONTROLLER_H
