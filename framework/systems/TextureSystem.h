//
// Created by drgio on 19/07/2023.
//

#ifndef DEMO_TEXTURESYSTEM_H
#define DEMO_TEXTURESYSTEM_H

#include <memory>
#include "../../Starter.hpp"

namespace fmwk {

    class BoundTexture{
    private:
        Texture _texture{};
        DescriptorSet _textureSet;
    public:
        BoundTexture(BaseProject *bp, char const* fileName, DescriptorSetLayout& textureSetLayout);
        void buildTextureSet(BaseProject* bp, DescriptorSetLayout& descriptorSetLayout);
        void clearTextureSet();
        void destroyTexture();
        Texture& getTexture();
        DescriptorSet& getDescriptorSet();
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
        void rebuildTextureDescriptorSets();
        void resetTextureDescriptorSets();
        void destroyTextures();
    };

} // fmwk

#endif //DEMO_TEXTURESYSTEM_H
