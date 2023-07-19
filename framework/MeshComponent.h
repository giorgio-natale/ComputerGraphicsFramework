//
// Created by drgio on 19/07/2023.
//

#ifndef DEMO_MESHCOMPONENT_H
#define DEMO_MESHCOMPONENT_H

#include "../Starter.hpp"
#include "Component.h"

namespace fmwk {

    class MeshComponent : public Component{
    private:
        BaseModel& _model;
    public:
        MeshComponent(std::string const& name, BaseModel& model);
        [[nodiscard]] BaseModel& getModel() const;
    };

} // fmwk

#endif //DEMO_MESHCOMPONENT_H
