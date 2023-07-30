//
// Created by drgio on 19/07/2023.
//

#include "MeshComponent.h"

namespace fmwk {
    MeshComponent::MeshComponent(TModel &model): Component("Mesh"), _model(model){}

    TModel &MeshComponent::getModel() const {
        return _model;
    }
} // fmwk