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

        void setMaxSpeed(float maxSpeed);

    private:
        Transform const& _cameraTransform;
        MazeRepresentation const& _mazeRepresentation;
        float _maxSpeed;
        bool _spacePressConsidered;

        glm::vec3 getDistanceFromCharacter(Square &square, glm::vec3 vec1);
    };

} // fmwk

#endif //DEMO_MAZECHARACTERCONTROLLER_H
