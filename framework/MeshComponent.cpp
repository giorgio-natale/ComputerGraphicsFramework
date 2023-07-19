//
// Created by drgio on 19/07/2023.
//

#include "MeshComponent.h"

namespace fmwk {
    MeshComponent::MeshComponent(const std::string &name, BaseModel &model): Component(name), _model(model){}

    BaseModel &MeshComponent::getModel() const {
        return _model;
    }
} // fmwk