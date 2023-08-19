//
// Created by drgio on 19/08/2023.
//

#ifndef DEMO_CHARACTER_H
#define DEMO_CHARACTER_H

#include "Blueprint.h"

namespace fmwk {

    class Character : public Blueprint{
    public:
        explicit Character(glm::vec3 position);

    protected:
        void buildEntity() override;

    private:
        glm::vec3 _position;
    };

} // fmwk

#endif //DEMO_CHARACTER_H
