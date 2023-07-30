//
// Created by drgio on 01/05/2023.
//

#ifndef A07_CAMERA_H
#define A07_CAMERA_H

#include <glm/glm.hpp>
#include "../scripts/Component.h"

namespace fmwk {

    class Camera : public Component{
    public:
        Camera(float nearPlane);

        ~Camera() override;

        virtual glm::mat4 getProjectionMatrix() = 0;
        [[nodiscard]] glm::mat4 getViewMatrix() const;

    protected:
        float _nearPlane;
    };

} // fmwk

#endif //A07_CAMERA_H
