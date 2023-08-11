//
// Created by drgio on 11/08/2023.
//

#ifndef DEMO_MAZECHARACTERCONTROLLER_H
#define DEMO_MAZECHARACTERCONTROLLER_H

#include "Component.h"
#include "../transform/Transform.h"
#include "MazeRepresentation.h"

namespace fmwk {

    class MazeCharacterController : public Component {
    public:
        MazeCharacterController(const Transform &cameraTransform, const MazeRepresentation &mazeRepresentation, float maxSpeed);

        void update() override;

    private:
        Transform const& _cameraTransform;
        MazeRepresentation const& _mazeRepresentation;
        float const _maxSpeed;
        bool _spacePressConsidered;
    };

} // fmwk

#endif //DEMO_MAZECHARACTERCONTROLLER_H
