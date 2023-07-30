//
// Created by drgio on 04/05/2023.
//

#ifndef A07_CAMERACONTROLLER_H
#define A07_CAMERACONTROLLER_H

#include "Component.h"
#include "../transform/Transform.h"

namespace fmwk {

    class CameraController : public Component{
    public:
        CameraController(
                const std::string &name,
                const Transform &characterTransform,
                float angularSpeed,
                float cameraDistance,
                float cameraHeight);

        void postUpdate() override;

    private:
        Transform const& _characterTransform;
        float const _angularSpeed;
        float const _cameraDistance;
        float const _cameraHeight;
        glm::vec3 _rotationAboutCenter = {0,0,0};
    };

} // fmwk

#endif //A07_CAMERACONTROLLER_H
