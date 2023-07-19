//
// Created by drgio on 19/07/2023.
//

#ifndef DEMO_TEXTURESYSTEM_H
#define DEMO_TEXTURESYSTEM_H

#include <memory>
#include "../../Starter.hpp"

namespace fmwk {

    struct BoundTexture{
        Texture texture{};
        DescriptorSet textureSet;
    };

    class TextureSystem {
    private:
        BaseProject *_bp;
        DescriptorSetLayout _textureSetLayout{};
        std::unordered_map<std::string, std::unique_ptr<BoundTexture>> _boundTextures;
    public:
        explicit TextureSystem(BaseProject *bp);
        void addTexture(std::string const& name, std::string const& fileName);
        BoundTexture& getBoundTextureByName(std::string const& name);
        DescriptorSetLayout& getTextureDescriptorSetLayout();
        void bootSystem();
    };

} // fmwk

#endif //DEMO_TEXTURESYSTEM_H
