
#ifndef DEMO_FINALBOSS_H
#define DEMO_FINALBOSS_H

#include "../Starter.hpp"

class FinalBoss : public BaseProject {
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

private:
    std::pair<std::vector<fmwk::VertexWithNormal>, std::vector<uint32_t>> buildMazeModel();

};

#endif //DEMO_FINALBOSS_H
