//
// Created by drgio on 28/08/2023.
//

#ifndef DEMO_POWERUPICON_H
#define DEMO_POWERUPICON_H

#include "Blueprint.h"

namespace fmwk {

    class PowerupIcon : public Blueprint{
    public:
        PowerupIcon(float topOffset, const std::string &textureName);

    protected:
        void buildEntity() override;

        float _topOffset;
        const std::string &_textureName;
    };

} // fmwk

#endif //DEMO_POWERUPICON_H
