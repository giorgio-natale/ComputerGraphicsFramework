//
// Created by drgio on 09/08/2023.
//

#ifndef DEMO_MAZEESCAPE_H
#define DEMO_MAZEESCAPE_H

#include "../Starter.hpp"

class MazeEscape : public BaseProject {
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
    std::pair<std::vector<fmwk::VertexWithNormal>, std::vector<uint32_t>> buildMazeModel();

};


#endif //DEMO_MAZEESCAPE_H
