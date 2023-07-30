//
// Created by drgio on 19/07/2023.
//

#ifndef DEMO_TEXTURECOMPONENT_H
#define DEMO_TEXTURECOMPONENT_H

#include "../../systems/TextureSystem.h"
#include "../scripts/Component.h"

namespace fmwk {

    class TextureComponent : public Component{
    private:
        BoundTexture& _boundTexture;
    public:
        explicit TextureComponent(BoundTexture& boundTexture);
        BoundTexture& getBoundTexture();
    };

} // fmwk

#endif //DEMO_TEXTURECOMPONENT_H
