#ifndef DEMO_MOVINGCUBECONTROLLER_H
#define DEMO_MOVINGCUBECONTROLLER_H

#include <glm/vec3.hpp>
#include "Component.h"

namespace fmwk {

    class MovingCubeController: public Component{
    private:
        glm::vec3 _targetPosition;
        float _speed;

    public:
        void update() override;

        explicit MovingCubeController(const std::string &name, glm::vec3 targetPosition, float speed);
    };

} // fmwk

#endif //DEMO_MOVINGCUBECONTROLLER_H
