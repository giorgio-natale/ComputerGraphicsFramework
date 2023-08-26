//
// Created by drgio on 24/08/2023.
//

#ifndef DEMO_FLOOR_H
#define DEMO_FLOOR_H

#include "Blueprint.h"

namespace fmwk {

    class Floor : public Blueprint{
    protected:
    public:
        Floor(float width, float height);

    protected:
        void buildEntity() override;
    private:
        float _width;
        float _height;
    };

} // fmwk

#endif //DEMO_FLOOR_H
