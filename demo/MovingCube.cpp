#include "MovingCube.h"
// This has been adapted from the Vulkan tutorial

#include "../Starter.hpp"
#include "../framework/GameEngine.h"
#include "../framework/components/mesh/MeshComponent.h"
#include "../framework/components/texture/TextureComponent.h"
#include "../framework/components/materials/DefaultMaterial.h"
#include "../framework/components/camera/PerspectiveCamera.h"
#include "../framework/components/scripts/CharacterController.h"
#include "../framework/components/scripts/CameraController.h"
#include "../framework/components/materials/ColorBlendComponent.h"
#include "../framework/components/scripts/CubeSpawner.h"
#include "../framework/components/materials/GGXMaterial.h"
#include "../framework/components/lights/DirectLightComponent.h"
#include "../framework/components/lights/PointLightComponent.h"
#include "../framework/components/lights/SpotLightComponent.h"
#include "../framework/components/materials/SimplePhongMaterial.h"
#include "../framework/components/scripts/MovingCubeController.h"


// The uniform buffer objects data structures
// Remember to use the correct alignas(...) value
//        float : alignas(4)
//        vec2  : alignas(8)
//        vec3  : alignas(16)
//        vec4  : alignas(16)
//        mat3  : alignas(16)
//        mat4  : alignas(16)


// The vertices data structures
// Example

void MovingCube::setWindowParameters() {
    // window size, titile and initial background
    windowWidth = 800;
    windowHeight = 600;
    windowTitle = "Moving Cube";
    windowResizable = GLFW_TRUE;
    initialBackgroundColor = {0.0f, 0.005f, 0.01f, 1.0f};

    // Descriptor pool sizes
    uniformBlocksInPool = 10000;
    texturesInPool = 10000;
    setsInPool = 10000;

    Ar = (float)windowWidth / (float)windowHeight;
}

// What to do when the window changes size
void MovingCube::onWindowResize(int w, int h) {
    Ar = (float)w / (float)h;
}

// Here you load and setup all your Vulkan Models and Textures.
// Here you also create your Descriptor set layouts and load the shaders for the pipelines
void MovingCube::localInit() {
    auto gameEngine = fmwk::GameEngine::getInstance();

    gameEngine->addModel("myCube", fmwk::VERTEX_WITH_NORMAL, "Models/Cube.obj");
    gameEngine->addTexture("cubeTexture", "textures/Checker.png");

    auto cameraEntity = std::make_unique<fmwk::Entity>("Camera", glm::vec3(0, 10, 0), fmwk::createQuat(fmwk::X, 270));
    auto cameraComponent = std::make_unique<fmwk::PerspectiveCamera>(0.1f, 100.0f, glm::radians(45.0f));
    cameraEntity->addComponent(std::move(cameraComponent));

    auto cubeEntity = std::make_unique<fmwk::Entity>("Cube");
    cubeEntity->addComponent(std::make_unique<fmwk::MeshComponent>(gameEngine->getModelByName("myCube")));
    cubeEntity->addComponent(std::make_unique<fmwk::TextureComponent>(gameEngine->getBoundTextureByName("cubeTexture")));
    cubeEntity->addComponent(std::make_unique<fmwk::DefaultMaterial>(1.5f));
    cubeEntity->addComponent(std::make_unique<fmwk::MovingCubeController>("MovingCubeController", glm::vec3(3,0,-2), 0.8f));

    gameEngine->addEntity(std::move(cameraEntity));
    gameEngine->addEntity(std::move(cubeEntity));

    // Init local variables
    gameEngine->provisionResources(false);
}

// Here you create your pipelines and Descriptor Sets!
void MovingCube::pipelinesAndDescriptorSetsInit() {
    auto gameEngine = fmwk::GameEngine::getInstance();
    gameEngine->rebuildResources();

}

// Here you destroy your pipelines and Descriptor Sets!
// All the object classes defined in Starter.hpp have a method .cleanup() for this purpose
void MovingCube::pipelinesAndDescriptorSetsCleanup() {
    std::cout << "PIPELINES AND DESCRIPTOR SETS CLEANUP CALLED" << std::endl;
    auto gameEngine = fmwk::GameEngine::getInstance();
    gameEngine->cleanupResources();

}

// Here you destroy all the Models, Texture and Desc. Set Layouts you created!
// All the object classes defined in Starter.hpp have a method .cleanup() for this purpose
// You also have to destroy the pipelines: since they need to be rebuilt, they have two different
// methods: .cleanup() recreates them, while .destroy() delete them completely
void MovingCube::localCleanup() {
    std::cout << "LOCAL CLEANUP CALLED" << std::endl;

}

// Here it is the creation of the command buffer:
// You send to the GPU all the objects you want to draw,
// with their buffers and textures

void MovingCube::populateCommandBuffer(VkCommandBuffer commandBuffer, int currentImage) {
    auto gameEngine = fmwk::GameEngine::getInstance();
    gameEngine->renderFrame(commandBuffer, currentImage);
}

// Here is where you update the uniforms.
// Very likely this will be where you will be writing the logic of your application.
void MovingCube::updateUniformBuffer(uint32_t currentImage) {
    auto gameEngine = fmwk::GameEngine::getInstance();
    gameEngine->logicUpdate();
    gameEngine->provisionResources(true);

    // Standard procedure to quit when the ESC key is pressed
    if(glfwGetKey(window, GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    gameEngine->updateGraphicResources(currentImage);
    // the .map() method of a DataSet object, requires the current image of the swap chain as first parameter
    // the second parameter is the pointer to the C++ data structure to transfer to the GPU
    // the third parameter is its size
    // the fourth parameter is the location inside the descriptor set of this uniform block
}