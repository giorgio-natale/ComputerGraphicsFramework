//
// Created by drgio on 19/07/2023.
//

#include "TextureComponent.h"

namespace fmwk {
    TextureComponent::TextureComponent(const std::string &name, BoundTexture &boundTexture): Component(name), _boundTexture(boundTexture){}

    BoundTexture &TextureComponent::getBoundTexture() {
        return _boundTexture;
    }
} // fmwk