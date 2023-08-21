//
// Created by drgio on 19/08/2023.
//

#ifndef DEMO_CHARACTER_H
#define DEMO_CHARACTER_H

#include "Blueprint.h"
#include "../components/scripts/MazeRepresentation.h"

namespace fmwk {

    class Character : public Blueprint{
    public:
        Character(glm::vec3 position, MazeRepresentation& mazeRepresentation);

    protected:
        void buildEntity() override;

    private:
        glm::vec3 _position;
        MazeRepresentation &_mazeRepresentation;
    };

} // fmwk

#endif //DEMO_CHARACTER_H
