//
// Created by drgio on 02/08/2023.
//

#ifndef DEMO_SIMPLECUBE_H
#define DEMO_SIMPLECUBE_H

#include "../Starter.hpp"

class SimpleCube : public BaseProject {
protected:

    // Current aspect ratio (used by the callback that resized the window
    float Ar;

    void setWindowParameters() override;
    void onWindowResize(int w, int h) override;
    void localInit() override;
    void pipelinesAndDescriptorSetsInit() override;
    void pipelinesAndDescriptorSetsCleanup() override;
    void localCleanup() override;
    void populateCommandBuffer(VkCommandBuffer commandBuffer, int currentImage) override;
    void updateUniformBuffer(uint32_t currentImage) override;

};

#endif //DEMO_SIMPLECUBE_H
