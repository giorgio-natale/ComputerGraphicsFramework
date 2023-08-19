//
// Created by drgio on 20/08/2023.
//

#ifndef DEMO_HEARTUI_H
#define DEMO_HEARTUI_H

#include "Blueprint.h"

namespace fmwk {

    class HeartUI : public Blueprint{
    public:
        explicit HeartUI(int heartIndex);

    protected:
        void buildEntity() override;
    private:
        int _heartIndex;
    };

} // fmwk

#endif //DEMO_HEARTUI_H
