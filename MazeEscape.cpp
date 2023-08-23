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
#include "MazeEscape.h"
#include "maze_generation/Maze.h"
#include "maze_generation/VertexDictionary.h"
#include "framework/components/scripts/BulletSpawner.h"
#include "framework/components/scripts/EnemyController.h"
#include "framework/components/scripts/EnemyCollisionBehaviour.h"
#include "framework/components/scripts/CharacterBulletSpawner.h"
#include "framework/blueprints/BlockMaze.h"
#include "framework/blueprints/Character.h"
#include "framework/blueprints/BasicEnemy.h"
#include "framework/blueprints/HeartUI.h"
#include "framework/blueprints/BossEnemy.h"
#include "framework/blueprints/Decoration.h"
#include "framework/components/scripts/FollowEntity.h"

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

void MazeEscape::setWindowParameters() {
    // window size, titile and initial background
    windowWidth = 800;
    windowHeight = 600;
    windowTitle = "Simple Cube";
    windowResizable = GLFW_TRUE;
    //initialBackgroundColor = {2.0f/255.0f,204.0f/255.0f,254.0f/255.0f, 1.0f};
    initialBackgroundColor = {17.0f/255.0f,17.0f/255.0f,61.0f/255.0f, 1.0f};


    // Descriptor pool sizes
    uniformBlocksInPool = 10000;
    texturesInPool = 10000;
    setsInPool = 10000;

    Ar = (float)windowWidth / (float)windowHeight;
}

// What to do when the window changes size
void MazeEscape::onWindowResize(int w, int h) {
    Ar = (float)w / (float)h;
}

// Here you load and setup all your Vulkan Models and Textures.
// Here you also create your Descriptor set layouts and load the shaders for the pipelines
void MazeEscape::localInit() {
    auto gameEngine = fmwk::GameEngine::getInstance();

    gameEngine->addModel("myCube", fmwk::VERTEX_WITH_NORMAL, "Models/Cube.obj");
    gameEngine->addModel("mySphere", fmwk::VERTEX_WITH_NORMAL_AND_TANGENT, "models/Sphere.gltf",
                         glm::vec3(0, 0, 0), glm::quat(1,0,0,0), glm::vec3(0.195f));
    gameEngine->addModel("smoothPumpkin", fmwk::VERTEX_WITH_NORMAL, "models/pumpkin.007_Mesh.5391.mgcg",
                         glm::vec3(0, -0.1f, 0), fmwk::createQuat(fmwk::Y, 180), glm::vec3(1.5f));
    gameEngine->addModel("ghost", fmwk::VERTEX_WITH_NORMAL, "Models/cast_Mesh.6268.mgcg",
                         glm::vec3(0, 0.5f, 0), fmwk::createQuat(fmwk::Y, 180), glm::vec3(1.5f));

    gameEngine->addModel("death", fmwk::VERTEX_WITH_NORMAL, "Models/death.obj",
                         glm::vec3(0, 0.5f, 0), glm::quat(1,0,0,0), glm::vec3(0.01f));

    gameEngine->addTexture("cubeTexture", "textures/Checker.png");
    gameEngine->addTexture("sphereTexture", "textures/Metals_09_basecolor.png");
    gameEngine->addTexture("sphereNormal", "textures/Metals_09_normal.png");
    gameEngine->addTexture("sphereMaterial", "textures/Metals_09_met_rough_ao.png");
    gameEngine->addTexture("mainMazeTexture", "textures/maze_main.png");
    gameEngine->addTexture("heart", "textures/heart.png");
    gameEngine->addTexture("bossBar0", "textures/bossBar0.png");
    gameEngine->addTexture("bossBar1", "textures/bossBar1.png");
    gameEngine->addTexture("bossBar2", "textures/bossBar2.png");
    gameEngine->addTexture("bossBar3", "textures/bossBar3.png");
    gameEngine->addTexture("bossBar4", "textures/bossBar4.png");
    gameEngine->addTexture("bossBar5", "textures/bossBar5.png");

    gameEngine->addTexture("dungeonTexture", "textures/Textures_Dungeon.png");
    gameEngine->addTexture("red", "textures/red.png");
    gameEngine->addTexture("white", "textures/white.jpg");


    gameEngine->addTexture("bushColor", "textures/Grass_001_COLOR.jpg");
    gameEngine->addTexture("bushNormal", "textures/Grass_001_NORM.jpg");
    gameEngine->addTexture("bushRoughness", "textures/Grass_001_ROUGH.jpg");

    gameEngine->addTexture("rockColor", "textures/Stylized_Stone_Floor_005_basecolor.jpg");
    //gameEngine->addTexture("rockNormal", "textures/Stylized_Stone_Floor_005_normal.jpg");
    gameEngine->addTexture("rockNormal", "textures/Metals_09_normal.png");

    gameEngine->addTexture("rockRoughness", "textures/Stylized_Stone_Floor_005_roughness.jpg");
    gameEngine->addTexture("death", "textures/death.png");


    gameEngine->addModel<fmwk::VertexOverlay>("spriteModel", fmwk::VERTEX_OVERLAY,
                                              {
                                                      {{0.0f, 0.0f}, {0,1}},
                                                      {{1.0f, 0.0f}, {1,1}},
                                                      {{1.0f, 1.0f}, {1,0}},
                                                      {{0.0f, 1.0f}, {0,0}}

                                              }
            , {0, 1, 2,  2, 3, 0}, glm::vec3(0,0,0), glm::quat(1,0,0,0), glm::vec3(1));



    fmwk::BlockMaze().addInstance();

   /*fmwk::Character(glm::vec3(87.0f,0.5f, -15.0f),
                    reinterpret_cast<fmwk::MazeRepresentation&>(gameEngine->getEntityByName("Maze").getComponentByName("MazeRepresentation"))).addInstance();*/


   fmwk::Character(glm::vec3(75.0f,0.5f, -87.0f),
              reinterpret_cast<fmwk::MazeRepresentation&>(gameEngine->getEntityByName("Maze").getComponentByName("MazeRepresentation"))).addInstance();

   fmwk::Decoration(glm::vec3(75.0f,0.5f, -87.0f), glm::quat(1, 0, 0, 0), glm::vec3(1), "ghost").addInstance();

    std::vector<glm::vec3> e1 = {
          glm::vec3(9, 0.5, -9),
          glm::vec3(27, 0.5, -9),
          glm::vec3(27, 0.5, -33),
          glm::vec3(15, 0.5, -33),
          glm::vec3(15, 0.5, -21),
          glm::vec3(9, 0.5, -21)
    };

    std::vector<glm::vec3> e2 = {
          glm::vec3(27, 0.5, -27),
          glm::vec3(39, 0.5, -27),
          glm::vec3(39, 0.5, -21),
          glm::vec3(63, 0.5, -21),
    };

    std::vector<glm::vec3> e3 = {
          glm::vec3(15, 0.5, -33),
          glm::vec3(15, 0.5, -39),
          glm::vec3(9, 0.5, -39),
          glm::vec3(9, 0.5, -51),
          glm::vec3(15, 0.5, -51),
          glm::vec3(15, 0.5, -63),
          glm::vec3(9, 0.5, -63),
    };
    std::vector<glm::vec3> e4 = {
            glm::vec3(9, 0.5, -63),
            glm::vec3(9, 0.5, -75),
            glm::vec3(15, 0.5, -75),
            glm::vec3(15, 0.5, -87),
            glm::vec3(9, 0.5, -87),
            glm::vec3(9, 0.5, -99)
    };

    std::vector<glm::vec3> e5 = {
            glm::vec3(63, 0.5, -9),
            glm::vec3(63, 0.5, -33),
            glm::vec3(75, 0.5, -33),
            glm::vec3(75, 0.5, -9),
    };

    std::vector<glm::vec3> e6 = {
            glm::vec3(63, 0.5, -33),
            glm::vec3(51, 0.5, -33),
            glm::vec3(51, 0.5, -39),
            glm::vec3(45, 0.5, -39),
            glm::vec3(45, 0.5, -51)
    };

    std::vector<glm::vec3> e7 = {
            glm::vec3(27, 0.5, -57),
            glm::vec3(39, 0.5, -57),
            glm::vec3(39, 0.5, -51),
            glm::vec3(69, 0.5, -51)
    };

    std::vector<glm::vec3> e8 = {
            glm::vec3(39, 0.5, -69),
            glm::vec3(51, 0.5, -69),
            glm::vec3(51, 0.5, -63),
            glm::vec3(63, 0.5, -63),
            glm::vec3(63, 0.5, -51)
    };

    std::vector<glm::vec3> e9 = {
            glm::vec3(63, 0.5, -81),
            glm::vec3(63, 0.5, -75),
            glm::vec3(75, 0.5, -75),
            glm::vec3(75, 0.5, -57),
            glm::vec3(87, 0.5, -57)
    };

    std::vector<glm::vec3> e10 = {
            glm::vec3(69, 0.5, -45),
            glm::vec3(111, 0.5, -45)
    };

    std::vector<glm::vec3> e11 = {
            glm::vec3(87, 0.5, -69),
            glm::vec3(87, 0.5, -75),
            glm::vec3(99, 0.5, -75),
            glm::vec3(99, 0.5, -57),
            glm::vec3(87, 0.5, -57),
    };

    std::vector<glm::vec3> e12 = {
            glm::vec3(99, 0.5, -33),
            glm::vec3(99, 0.5, -21),
            glm::vec3(111, 0.5, -21),
            glm::vec3(111, 0.5, -51),
    };

    std::vector<glm::vec3> e13 = {
            glm::vec3(99, 0.5, -9),
            glm::vec3(111, 0.5, -9),
            glm::vec3(111, 0.5, -15),
            glm::vec3(129, 0.5, -15),
            glm::vec3(129, 0.5, -27)
    };

    std::vector<glm::vec3> e14 = {
            glm::vec3(123, 0.5, -27),
            glm::vec3(123, 0.5, -39),
            glm::vec3(129, 0.5, -39),
            glm::vec3(129, 0.5, -51),
            glm::vec3(123, 0.5, -51),
            glm::vec3(123, 0.5, -63)
    };

    std::vector<glm::vec3> e15 = {
            glm::vec3(111, 0.5, -63),
            glm::vec3(111, 0.5, -75),
            glm::vec3(129, 0.5, -75),
            glm::vec3(129, 0.5, -63),
            glm::vec3(123, 0.5, -63)
    };

    std::vector<glm::vec3> e16 = {
            glm::vec3(33, 0.5, -99),
            glm::vec3(33, 0.5, -93),
            glm::vec3(39, 0.5, -93),
            glm::vec3(39, 0.5, -99),
            glm::vec3(51, 0.5, -99),
            glm::vec3(51, 0.5, -93)
    };

    std::vector<glm::vec3> e17 = {
            glm::vec3(57, 0.5, -117),
            glm::vec3(57, 0.5, -105),
            glm::vec3(75, 0.5, -105),
            glm::vec3(75, 0.5, -117),
            glm::vec3(69, 0.5, -117)
    };

    fmwk::BasicEnemy(e1).addInstance();
    fmwk::BasicEnemy(e2).addInstance();
    fmwk::BasicEnemy(e3).addInstance();
    fmwk::BasicEnemy(e4).addInstance();
    fmwk::BasicEnemy(e5).addInstance();
    fmwk::BasicEnemy(e6).addInstance();
    fmwk::BasicEnemy(e7).addInstance();
    fmwk::BasicEnemy(e8).addInstance();
    fmwk::BasicEnemy(e9).addInstance();
    fmwk::BasicEnemy(e10).addInstance();
    fmwk::BasicEnemy(e11).addInstance();
    fmwk::BasicEnemy(e12).addInstance();
    fmwk::BasicEnemy(e13).addInstance();
    fmwk::BasicEnemy(e14).addInstance();
    fmwk::BasicEnemy(e15).addInstance();
    fmwk::BasicEnemy(e16).addInstance();
    fmwk::BasicEnemy(e17).addInstance();

    fmwk::BossEnemy(gameEngine->getEntityByName("Character").getTransform()).addInstance();


    /*auto lightEntity = std::make_unique<fmwk::Entity>("LightEntity", glm::vec3(0,3,0), glm::rotate(glm::quat(1,0,0,0), glm::radians(-90.0f), fmwk::X));
    lightEntity->addComponent(std::make_unique<fmwk::DirectLightComponent>("DirectLight", glm::normalize(glm::vec3(1, -1, 0)), glm::vec4(0.5)));
    lightEntity->addComponent(std::make_unique<fmwk::DirectLightComponent>("DirectLight2", glm::normalize(glm::vec3(-1, 0, 0)), glm::vec4(0.5)));
    lightEntity->addComponent(std::make_unique<fmwk::DirectLightComponent>("DirectLight3", glm::normalize(glm::vec3(0, -1, -1)), glm::vec4(0.5)));
    lightEntity->addComponent(std::make_unique<fmwk::DirectLightComponent>("DirectLight4", glm::normalize(glm::vec3(0, -1, 1)), glm::vec4(0.5)));
    */

    auto lightEntity = std::make_unique<fmwk::Entity>("LightEntity", glm::vec3(75.0f,0.5f, -87.0f), glm::quat(1,0,0,0));
    lightEntity->addComponent(std::make_unique<fmwk::DirectLightComponent>("DirectLight", glm::normalize(glm::vec3(0, -1, 0)), glm::vec4(0.5)));
    lightEntity->addComponent(std::make_unique<fmwk::FollowEntity>(gameEngine->getEntityByName("Character").getTransform()));
    lightEntity->addComponent(std::make_unique<fmwk::SpotLightComponent>("characterTorch", glm::vec4(1.0f), 2.0f, 4.0f, 0.8f, 0.85f));
    //lightEntity->addComponent(std::make_unique<fmwk::PointLightComponent>("characterTorch", glm::vec4(1.0f), 2.0f, 6.0f));


    gameEngine->addEntity(std::move(lightEntity));


    // Init local variables
    gameEngine->provisionResources(false);
}

// Here you create your pipelines and Descriptor Sets!
void MazeEscape::pipelinesAndDescriptorSetsInit() {
auto gameEngine = fmwk::GameEngine::getInstance();
gameEngine->rebuildResources();

}

// Here you destroy your pipelines and Descriptor Sets!
// All the object classes defined in Starter.hpp have a method .cleanup() for this purpose
void MazeEscape::pipelinesAndDescriptorSetsCleanup() {
std::cout << "PIPELINES AND DESCRIPTOR SETS CLEANUP CALLED" << std::endl;
auto gameEngine = fmwk::GameEngine::getInstance();
gameEngine->cleanupResources();

}

// Here you destroy all the Models, Texture and Desc. Set Layouts you created!
// All the object classes defined in Starter.hpp have a method .cleanup() for this purpose
// You also have to destroy the pipelines: since they need to be rebuilt, they have two different
// methods: .cleanup() recreates them, while .destroy() delete them completely
void MazeEscape::localCleanup() {
std::cout << "LOCAL CLEANUP CALLED" << std::endl;

}

// Here it is the creation of the command buffer:
// You send to the GPU all the objects you want to draw,
// with their buffers and textures

void MazeEscape::populateCommandBuffer(VkCommandBuffer commandBuffer, int currentImage) {
auto gameEngine = fmwk::GameEngine::getInstance();
gameEngine->renderFrame(commandBuffer, currentImage);
}

// Here is where you update the uniforms.
// Very likely this will be where you will be writing the logic of your application.
void MazeEscape::updateUniformBuffer(uint32_t currentImage) {
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
