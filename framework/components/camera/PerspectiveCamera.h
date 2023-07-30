//
// Created by drgio on 03/05/2023.
//

#ifndef A07_PERSPECTIVECAMERA_H
#define A07_PERSPECTIVECAMERA_H

#include "Camera.h"

namespace fmwk {

    class PerspectiveCamera : public Camera{
    public:

        PerspectiveCamera( float nearPlane, float farPlane, float fovY);
        glm::mat4 getProjectionMatrix() override;
        ~PerspectiveCamera() override;
    private:

    protected:
        float _farPlane;
        float _fovY;
    };

} // fmwk

#endif //A07_PERSPECTIVECAMERA_H
