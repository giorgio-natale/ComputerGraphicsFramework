//
// Created by drgio on 19/07/2023.
//

#ifndef DEMO_MESHCOMPONENT_H
#define DEMO_MESHCOMPONENT_H

#include "../../../Starter.hpp"
#include "../scripts/Component.h"
#include "../../systems/ModelSystem.h"

namespace fmwk {

    class MeshComponent : public Component{
    private:
        TModel& _model;
    public:
        explicit MeshComponent(TModel& model);
        [[nodiscard]] TModel& getModel() const;
    };

} // fmwk

#endif //DEMO_MESHCOMPONENT_H
