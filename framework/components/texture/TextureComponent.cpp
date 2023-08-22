//
// Created by drgio on 19/07/2023.
//

#include "TextureComponent.h"

namespace fmwk {
    TextureComponent::TextureComponent(BoundTexture &boundTexture): Component("Texture"), _boundTexture(boundTexture){}

    BoundTexture &TextureComponent::getBoundTexture() {
        return _boundTexture;
    }

    void TextureComponent::setBoundTexture(BoundTexture &boundTexture) {
        _boundTexture = boundTexture;
    }
} // fmwk