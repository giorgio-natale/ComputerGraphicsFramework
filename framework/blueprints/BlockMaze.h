//
// Created by drgio on 19/08/2023.
//

#ifndef DEMO_BLOCKMAZE_H
#define DEMO_BLOCKMAZE_H

#include "Blueprint.h"

namespace fmwk {

    class BlockMaze : public Blueprint{
    protected:
        void buildEntity() override;
    };

} // fmwk

#endif //DEMO_BLOCKMAZE_H
