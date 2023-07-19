//
// Created by drgio on 19/07/2023.
//

#include "TextureSystem.h"

namespace fmwk {

    TextureSystem::TextureSystem(BaseProject *bp) {
        _bp = bp;
    }

    void TextureSystem::addTexture(const std::string &name, const std::string &fileName) {
        if(_boundTextures.find(name) != _boundTextures.end())
            throw std::runtime_error("Could not add texture with name '" + name + "' because there was another texture with the same name");

        std::unique_ptr<BoundTexture> boundTexture = std::make_unique<BoundTexture>();

        boundTexture->texture.init(_bp, fileName.c_str());
        boundTexture->textureSet.init(_bp, &_textureSetLayout, { {0, TEXTURE, 0, &boundTexture->texture}});

        _boundTextures.insert({name, std::move(boundTexture)});
    }

    BoundTexture& TextureSystem::getBoundTextureByName(const std::string &name) {
        auto itr = _boundTextures.find(name);
        if (itr == _boundTextures.end())
            throw std::runtime_error("Could not find texture with name '" + name + "'");
        return *(_boundTextures.find(name)->second);
    }

    DescriptorSetLayout &TextureSystem::getTextureDescriptorSetLayout() {
        return _textureSetLayout;
    }

    void TextureSystem::bootSystem() {
        _textureSetLayout.init(_bp, {{0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT}});
    }
} // fmwk