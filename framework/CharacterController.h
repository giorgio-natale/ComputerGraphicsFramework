//
// Created by drgio on 03/05/2023.
//

#ifndef A07_CHARACTERCONTROLLER_H
#define A07_CHARACTERCONTROLLER_H

#include "Component.h"
#include "Transform.h"

namespace fmwk {

    class CharacterController : public Component{
    public:
        CharacterController(const std::string &name, const Transform &cameraTransform, float maxSpeed);
        void update() override;

    private:
        Transform const& _cameraTransform;
        float const _maxSpeed;
    };

} // fmwk

#endif //A07_CHARACTERCONTROLLER_H
