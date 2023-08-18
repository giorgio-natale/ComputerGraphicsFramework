//
// Created by drgio on 16/08/2023.
//

#ifndef DEMO_SPRITE_H
#define DEMO_SPRITE_H

#include "../scripts/Component.h"
#include "../../../Starter.hpp"
#include "SpriteUtils.h"
#include <glm/vec2.hpp>
#include <glm/ext/matrix_float3x3.hpp>

namespace fmwk {
    struct SpriteUniformBlock{
        alignas(16) glm::mat4 spriteTransform;
    };

    class Sprite : public Component{
    public:
        explicit Sprite(glm::vec2 preferredSize, const std::vector<UiAnchor>& anchors, bool stretchable);

        [[nodiscard]] bool isProvisioned() const override;
        void provision(DescriptorSet* descriptorSet);
        DescriptorSet& getDescriptorSet();
        void updateDescriptorSet(int currentImage, float virtualScreenWidthUnits, float aspectRatio);

    private:
        bool _alreadyProvisioned;
        DescriptorSet* _descriptorSet;

        glm::vec2 _preferredSize;
        std::vector<UiAnchor> _anchors;
        bool _stretchable;
    };

} // fmwk

#endif //DEMO_SPRITE_H
