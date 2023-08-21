#include "MazeGeneration.h"
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
#include "../maze_generation/Maze.h"
#include "../maze_generation/VertexDictionary.h"
#include "../framework/components/scripts/MazeCharacterController.h"
#include "SimpleCube.h"
#include "FinalBoss.h"
#include "../framework/components/scripts/MazeCharacterController.h"
#include "../framework/components/scripts/FinalBossController.h"


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

void FinalBoss::setWindowParameters() {
    // window size, titile and initial background
    windowWidth = 800;
    windowHeight = 600;
    windowTitle = "Simple Cube";
    windowResizable = GLFW_TRUE;
    initialBackgroundColor = {0.0f, 0.005f, 0.01f, 1.0f};

    // Descriptor pool sizes
    uniformBlocksInPool = 10000;
    texturesInPool = 10000;
    setsInPool = 10000;

    Ar = (float)windowWidth / (float)windowHeight;
}

// What to do when the window changes size
void FinalBoss::onWindowResize(int w, int h) {
    Ar = (float)w / (float)h;
}

// Here you load and setup all your Vulkan Models and Textures.
// Here you also create your Descriptor set layouts and load the shaders for the pipelines
void FinalBoss::localInit() {
    /*
    auto gameEngine = fmwk::GameEngine::getInstance();

    gameEngine->addModel("myCube", fmwk::VERTEX_WITH_NORMAL, "Models/Cube.obj");
    gameEngine->addModel("mySphere", fmwk::VERTEX_WITH_NORMAL_AND_TANGENT, "models/Sphere.gltf");
    gameEngine->addTexture("cubeTexture", "textures/Checker.png");
    gameEngine->addTexture("mainMazeTexture", "textures/maze_main.png");
    gameEngine->addTexture("sphereTexture", "textures/Metals_09_basecolor.png");
    gameEngine->addTexture("sphereNormal", "textures/Metals_09_normal.png");
    gameEngine->addTexture("sphereMaterial", "textures/Metals_09_met_rough_ao.png");
    //SETUP SCENE

    //LOAD MAZE
    auto mazeEntity = std::make_unique<fmwk::Entity>("Maze");
    char *mazeIn[] = {

            (char*)("### ###########"),
            (char*)("# #   #       #"),
            (char*)("# # ### ### ###"),
            (char*)("#   #     #   #"),
            (char*)("# ### ####### #"),
            (char*)("#   # #       #"),
            (char*)("### ### #######"),
            (char*)("# #   #       #"),
            (char*)("## ###########"),
            (char*)("#             #"),
            (char*)("#             #"),
            (char*)("#             #"),
            (char*)("#             #"),
            (char*)("#             #"),
            (char*)("############ ##")
    };
    int row = 15;
    int col = 15;
    auto mazeRepresentation = std::make_unique<fmwk::MazeRepresentation>(row, col, 6.0f, mazeIn);
    auto [vertices, faces] = mazeRepresentation->buildMesh();
    gameEngine->addModel<fmwk::VertexWithNormal>("MazeModel", fmwk::VERTEX_WITH_NORMAL, vertices, faces, glm::vec3(0), glm::quat(1,0,0,0), glm::vec3(1));

    mazeEntity->addComponent(std::make_unique<fmwk::MeshComponent>(gameEngine->getModelByName("MazeModel")));
    mazeEntity->addComponent(std::make_unique<fmwk::TextureComponent>(gameEngine->getBoundTextureByName("mainMazeTexture")));
    mazeEntity->addComponent(std::make_unique<fmwk::SimplePhongMaterial>());
    mazeEntity->addComponent(std::move(mazeRepresentation));


    auto cameraEntity = std::make_unique<fmwk::Entity>("Camera", glm::vec3(0, 10, 0), fmwk::createQuat(fmwk::X, 270));
    auto cameraComponent = std::make_unique<fmwk::PerspectiveCamera>(0.1f, 50.0f, glm::radians(45.0f));
    cameraEntity->addComponent(std::move(cameraComponent));

    auto sphereEntity = std::make_unique<fmwk::Entity>("Sphere", glm::vec3(75,0.5f,-3),glm::quat(1, 0, 0, 0));
    sphereEntity->getTransform().setScale(glm::vec3(0.5f, 0.5f, 0.5f));
    sphereEntity->addComponent(std::make_unique<fmwk::MeshComponent>(gameEngine->getModelByName("mySphere")));
    sphereEntity->addComponent(std::make_unique<fmwk::TextureComponent>(gameEngine->getBoundTextureByName("sphereTexture")));
    sphereEntity->addComponent(std::make_unique<fmwk::DefaultMaterial>(1.0));
    sphereEntity->addComponent(std::make_unique<fmwk::MazeCharacterController>(cameraEntity->getTransform(),
                                                                               dynamic_cast<fmwk::MazeRepresentation&>(mazeEntity->getComponentByName(
                                                                                       "MazeRepresentation")), 4.0f));

    cameraEntity->addComponent(std::make_unique<fmwk::CameraController>("CameraController", sphereEntity->getTransform(), glm::radians(120.0f), 3.0f, 0.25f));

    auto cubeEntity = std::make_unique<fmwk::Entity>("FinalBoss");
    cubeEntity->getTransform().setScale(glm::vec3(0.5f, 0.5f, 0.5f));
    cubeEntity->addComponent(std::make_unique<fmwk::MeshComponent>(gameEngine->getModelByName("myCube")));
    cubeEntity->addComponent(std::make_unique<fmwk::TextureComponent>(gameEngine->getBoundTextureByName("cubeTexture")));
    cubeEntity->addComponent(std::make_unique<fmwk::DefaultMaterial>(1.0));
    cubeEntity->addComponent(std::make_unique<fmwk::FinalBossController>(glm::vec3(45,0.5f,-21), sphereEntity->getTransform()));

    gameEngine->addEntity(std::move(sphereEntity));
    gameEngine->addEntity(std::move(cubeEntity));
    gameEngine->addEntity(std::move(cameraEntity));
    gameEngine->addEntity(std::move(mazeEntity));

    auto lightEntity = std::make_unique<fmwk::Entity>("LightEntity", glm::vec3(0,3,0), glm::rotate(glm::quat(1,0,0,0), glm::radians(-90.0f), fmwk::X));
    lightEntity->addComponent(std::make_unique<fmwk::DirectLightComponent>("DirectLight1", glm::vec3(1, 0, 0), glm::vec4(1)));

    gameEngine->addEntity(std::move(lightEntity));



*/
    // Init local variables
    //gameEngine->provisionResources(false);
}

// Here you create your pipelines and Descriptor Sets!
void FinalBoss::pipelinesAndDescriptorSetsInit() {
    auto gameEngine = fmwk::GameEngine::getInstance();
    gameEngine->rebuildResources();

}

// Here you destroy your pipelines and Descriptor Sets!
// All the object classes defined in Starter.hpp have a method .cleanup() for this purpose
void FinalBoss::pipelinesAndDescriptorSetsCleanup() {
    std::cout << "PIPELINES AND DESCRIPTOR SETS CLEANUP CALLED" << std::endl;
    auto gameEngine = fmwk::GameEngine::getInstance();
    gameEngine->cleanupResources();

}

// Here you destroy all the Models, Texture and Desc. Set Layouts you created!
// All the object classes defined in Starter.hpp have a method .cleanup() for this purpose
// You also have to destroy the pipelines: since they need to be rebuilt, they have two different
// methods: .cleanup() recreates them, while .destroy() delete them completely
void FinalBoss::localCleanup() {
    std::cout << "LOCAL CLEANUP CALLED" << std::endl;

}

// Here it is the creation of the command buffer:
// You send to the GPU all the objects you want to draw,
// with their buffers and textures

void FinalBoss::populateCommandBuffer(VkCommandBuffer commandBuffer, int currentImage) {
    auto gameEngine = fmwk::GameEngine::getInstance();
    gameEngine->renderFrame(commandBuffer, currentImage);
}

// Here is where you update the uniforms.
// Very likely this will be where you will be writing the logic of your application.
void FinalBoss::updateUniformBuffer(uint32_t currentImage) {
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
