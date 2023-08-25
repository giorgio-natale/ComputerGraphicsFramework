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
#include "framework/blueprints/PowerUpCube.h"

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

    gameEngine->addModel("myCube", fmwk::VERTEX_WITH_NORMAL, "Models/Cube.obj",
                         glm::vec3(0, 0.4, 0), glm::quat(1,0,0,0), glm::vec3(0.4f));
    gameEngine->addModel("mySphere", fmwk::VERTEX_WITH_NORMAL_AND_TANGENT, "models/Sphere.gltf",
                         glm::vec3(0, 0, 0), glm::quat(1,0,0,0), glm::vec3(0.195f));
    gameEngine->addModel("shieldSphere", fmwk::VERTEX_WITH_NORMAL_AND_TANGENT, "models/Sphere.gltf",
                         glm::vec3(0, 0, 0), glm::quat(1,0,0,0), glm::vec3(0.7f));
    gameEngine->addModel("smoothPumpkin", fmwk::VERTEX_WITH_NORMAL, "models/pumpkin.007_Mesh.5391.mgcg",
                         glm::vec3(0, -0.1f, 0), fmwk::createQuat(fmwk::Y, 180), glm::vec3(1.5f));
    gameEngine->addModel("ghost", fmwk::VERTEX_WITH_NORMAL, "Models/cast_Mesh.6268.mgcg",
                         glm::vec3(0, 0.5f, 0), fmwk::createQuat(fmwk::Y, 180), glm::vec3(1.5f));
    gameEngine->addModel("death", fmwk::VERTEX_WITH_NORMAL, "Models/death.obj",
                         glm::vec3(0, 0.5f, 0), glm::quat(1,0,0,0), glm::vec3(0.01f));

    // Decorations
    gameEngine->addModel("streetLamp", fmwk::VERTEX_WITH_NORMAL, "models/decoration.002_Mesh.7105.mgcg",
                         glm::vec3(0, 0, 0), glm::quat(1,0,0,0), glm::vec3(0.5f));
    gameEngine->addModel("crucifiedSkeleton", fmwk::VERTEX_WITH_NORMAL, "models/bones.001_Mesh.7532.mgcg",
                         glm::vec3(0, 0, 0), glm::quat(1,0,0,0), glm::vec3(0.5f));
    gameEngine->addModel("hangingSkeleton", fmwk::VERTEX_WITH_NORMAL, "models/bones.033_Mesh.700.mgcg",
                         glm::vec3(0, 0, 0), glm::quat(1,0,0,0), glm::vec3(0.5f));
    gameEngine->addModel("chain", fmwk::VERTEX_WITH_NORMAL, "models/decoration.005_Mesh.6547.mgcg",
                         glm::vec3(0, 0, 0), glm::quat(1,0,0,0), glm::vec3(0.5f));
    gameEngine->addModel("woodBeam", fmwk::VERTEX_WITH_NORMAL, "models/decoration.015_Mesh.6804.mgcg",
                         glm::vec3(0, 0, 0), glm::quat(1,0,0,0), glm::vec3(0.5f));
    gameEngine->addModel("crossX", fmwk::VERTEX_WITH_NORMAL, "models/decoration.019_Mesh.4909.mgcg",
                         glm::vec3(0, 0, 0), glm::quat(1,0,0,0), glm::vec3(0.5f));
    gameEngine->addModel("crossT", fmwk::VERTEX_WITH_NORMAL, "models/decoration.020_Mesh.4906.mgcg",
                         glm::vec3(0, 0, 0), glm::quat(1,0,0,0), glm::vec3(0.5f));
    gameEngine->addModel("bossTotem", fmwk::VERTEX_WITH_NORMAL, "models/decoration.026_Mesh.6000.mgcg",
                         glm::vec3(0, 0, 0), glm::quat(1,0,0,0), glm::vec3(0.5f));
    gameEngine->addModel("wallDecor", fmwk::VERTEX_WITH_NORMAL, "models/tunnel.033_Mesh.6508.mgcg",
                         glm::vec3(0, 0, 0), glm::quat(1,0,0,0), glm::vec3(0.5f));
    gameEngine->addModel("wallLamp", fmwk::VERTEX_WITH_NORMAL, "models/light.009_Mesh.6851.mgcg",
                         glm::vec3(0, 0, 0), glm::quat(1,0,0,0), glm::vec3(0.5f));
    gameEngine->addModel("wallChains", fmwk::VERTEX_WITH_NORMAL, "models/decoration.025_Mesh.5810.mgcg",
                         glm::vec3(0, 0, 0), glm::quat(1,0,0,0), glm::vec3(0.5f));
    gameEngine->addModel("wallCircle", fmwk::VERTEX_WITH_NORMAL, "models/decoration.021_Mesh.268.mgcg",
                         glm::vec3(0, 0, 0), glm::quat(1,0,0,0), glm::vec3(0.5f));
    gameEngine->addModel("wallShield", fmwk::VERTEX_WITH_NORMAL, "models/prop.014_Mesh.7789.mgcg",
                         glm::vec3(0, 0, 0), glm::quat(1,0,0,0), glm::vec3(0.5f));
    gameEngine->addModel("shelf1", fmwk::VERTEX_WITH_NORMAL, "models/prop.016_Mesh.6779.mgcg",
                         glm::vec3(0, 0, 0), glm::quat(1,0,0,0), glm::vec3(0.5f));
    gameEngine->addModel("shelf2", fmwk::VERTEX_WITH_NORMAL, "models/prop.034_Mesh.7986.mgcg",
                         glm::vec3(0, 0, 0), glm::quat(1,0,0,0), glm::vec3(0.5f));
    gameEngine->addModel("shelf3", fmwk::VERTEX_WITH_NORMAL, "models/prop.035_Mesh.6860.mgcg",
                         glm::vec3(0, 0, 0), glm::quat(1,0,0,0), glm::vec3(0.5f));
    gameEngine->addModel("closedWindow", fmwk::VERTEX_WITH_NORMAL, "models/prop.032_Mesh.7038.mgcg",
                         glm::vec3(0, 0, 0), glm::quat(1,0,0,0), glm::vec3(0.5f));

    gameEngine->addTexture("cubeTexture", "textures/Checker.png");
    gameEngine->addTexture("powerUpBullet", "textures/powerUpBullet.png");
    gameEngine->addTexture("powerUpLife", "textures/powerUpLife.png");
    gameEngine->addTexture("powerUpShield", "textures/powerUpShield.png");
    gameEngine->addTexture("powerUpSpeedUp", "textures/powerUpSpeedUp.png");
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
    gameEngine->addTexture("bossBar6", "textures/bossBar6.png");
    gameEngine->addTexture("bossBar7", "textures/bossBar7.png");
    gameEngine->addTexture("bossBar8", "textures/bossBar8.png");
    gameEngine->addTexture("bossBar9", "textures/bossBar9.png");
    gameEngine->addTexture("bossBar10", "textures/bossBar10.png");
    gameEngine->addTexture("gameOver", "textures/gameOver.PNG");
    gameEngine->addTexture("youWin", "textures/youWin.PNG");


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

   fmwk::Character(glm::vec3(87.0f,0.5f, -47.0f),
                    reinterpret_cast<fmwk::MazeRepresentation&>(gameEngine->getEntityByName("Maze").getComponentByName("MazeRepresentation"))).addInstance();


//   fmwk::Character(glm::vec3(75.0f,0.5f, -87.0f),
//              reinterpret_cast<fmwk::MazeRepresentation&>(gameEngine->getEntityByName("Maze").getComponentByName("MazeRepresentation"))).addInstance();

    fmwk::Decoration(glm::vec3(84.5f,2, -47.5f), fmwk::createQuat(fmwk::Y, 135), glm::vec3(2), "streetLamp").addInstance();
    fmwk::Decoration(glm::vec3(108.5f,2, -15.0f), fmwk::createQuat(fmwk::Y, 180), glm::vec3(2), "streetLamp").addInstance();
    fmwk::Decoration(glm::vec3(51.0f,2, -30.5f), fmwk::createQuat(fmwk::Y, 270), glm::vec3(2), "streetLamp").addInstance();
    fmwk::Decoration(glm::vec3(21.0f,2, -35.5f), fmwk::createQuat(fmwk::Y, 90), glm::vec3(2), "streetLamp").addInstance();
    fmwk::Decoration(glm::vec3(53.5f,2, -99.0f), fmwk::createQuat(fmwk::Y, 0), glm::vec3(2), "streetLamp").addInstance();

    fmwk::Decoration(glm::vec3(117,3, -96.5f), fmwk::createQuat(fmwk::Y, 180), glm::vec3(3), "wallLamp").addInstance();
    fmwk::Decoration(glm::vec3(99,3, -107.5), fmwk::createQuat(fmwk::Y, 0), glm::vec3(3), "wallLamp").addInstance();

    fmwk::Decoration(glm::vec3(95.7f,2.7, -92.2f), fmwk::createQuat(fmwk::Y, -90), glm::vec3(3), "crucifiedSkeleton").addInstance();
    fmwk::Decoration(glm::vec3(95.9f,3.5, -92.2f), fmwk::createQuat(fmwk::Y, -90, fmwk::X, 90), glm::vec3(3), "crossT").addInstance();
    fmwk::Decoration(glm::vec3(95.7f,1, -96.0f), fmwk::createQuat(fmwk::Y, -90), glm::vec3(2.5), "hangingSkeleton").addInstance();
    fmwk::Decoration(glm::vec3(96.0f,4, -96.0f), fmwk::createQuat(fmwk::Y, -90), glm::vec3(2), "woodBeam").addInstance();
    fmwk::Decoration(glm::vec3(95.7f,3.4, -99.8f), fmwk::createQuat(fmwk::Y, -90,  fmwk::Z, 180), glm::vec3(3), "crucifiedSkeleton").addInstance();
    fmwk::Decoration(glm::vec3(95.9f,2.5, -99.8f), fmwk::createQuat(fmwk::Y, -90, fmwk::X, 270, fmwk::Z, 180), glm::vec3(3), "crossT").addInstance();
    fmwk::Decoration(glm::vec3(113.7f,2.7, -104.2f), fmwk::createQuat(fmwk::Y, -90), glm::vec3(3), "crucifiedSkeleton").addInstance();
    fmwk::Decoration(glm::vec3(113.9f,3.5, -104.2f), fmwk::createQuat(fmwk::Y, -90, fmwk::X, 90), glm::vec3(3), "crossT").addInstance();
    fmwk::Decoration(glm::vec3(113.7f,1, -108.0f), fmwk::createQuat(fmwk::Y, -90), glm::vec3(2.5), "hangingSkeleton").addInstance();
    fmwk::Decoration(glm::vec3(114.0f,4, -108.0f), fmwk::createQuat(fmwk::Y, -90), glm::vec3(2), "woodBeam").addInstance();
    fmwk::Decoration(glm::vec3(113.7f,3.4, -111.8f), fmwk::createQuat(fmwk::Y, -90, fmwk::Z, 180), glm::vec3(3), "crucifiedSkeleton").addInstance();
    fmwk::Decoration(glm::vec3(113.9f,2.5, -111.8f), fmwk::createQuat(fmwk::Y, -90, fmwk::X, 270, fmwk::Z, 180), glm::vec3(3), "crossT").addInstance();

    fmwk::Decoration(glm::vec3(105.0f,0, -45.0f), fmwk::createQuat(fmwk::Y, 90), glm::vec3(2), "chain").addInstance();
    fmwk::Decoration(glm::vec3(129.0f,0, -21.0f), fmwk::createQuat(fmwk::Y, 180), glm::vec3(2), "chain").addInstance();
    fmwk::Decoration(glm::vec3(69.0f,0, -75.0f), fmwk::createQuat(fmwk::Y, 270), glm::vec3(2), "chain").addInstance();
    fmwk::Decoration(glm::vec3(57.0f,0, -63.0f), fmwk::createQuat(fmwk::Y, 90), glm::vec3(2), "chain").addInstance();
    fmwk::Decoration(glm::vec3(75.0f,0, -33.0f), fmwk::createQuat(fmwk::Y, 90), glm::vec3(2), "chain").addInstance();
    fmwk::Decoration(glm::vec3(21.0f,0, -9.0f), fmwk::createQuat(fmwk::Y, -90), glm::vec3(2), "chain").addInstance();
    fmwk::Decoration(glm::vec3(9.0f,0, -69.0f), fmwk::createQuat(fmwk::Y, 0), glm::vec3(2), "chain").addInstance();

    fmwk::Decoration(glm::vec3(113.5,1.5, -90.0f), fmwk::createQuat(fmwk::Y, -90), glm::vec3(2), "bossTotem").addInstance();
    fmwk::Decoration(glm::vec3(108,3, -119.8f), fmwk::createQuat(fmwk::Y, 0), glm::vec3(4), "crossX").addInstance();

    fmwk::Decoration(glm::vec3(87,0, -59.9f), fmwk::createQuat(fmwk::Y, 0), glm::vec3(5,5,4), "wallDecor").addInstance();
    fmwk::Decoration(glm::vec3(66.1f,0, -45), fmwk::createQuat(fmwk::Y, 90), glm::vec3(4,5,5), "wallDecor").addInstance();
    fmwk::Decoration(glm::vec3(36.1f,0, -51), fmwk::createQuat(fmwk::Y, 90), glm::vec3(4,5,5), "wallDecor").addInstance();
    fmwk::Decoration(glm::vec3(24.1f,0, -81), fmwk::createQuat(fmwk::Y, 90), glm::vec3(4,5,5), "wallDecor").addInstance();
    fmwk::Decoration(glm::vec3(111,0, -6.1), fmwk::createQuat(fmwk::Y, 0), glm::vec3(5,5,4), "wallDecor").addInstance();
    fmwk::Decoration(glm::vec3(129,0, -29.9), fmwk::createQuat(fmwk::Y, 0), glm::vec3(4,5,4), "wallDecor").addInstance();
    fmwk::Decoration(glm::vec3(108.1,0, -75), fmwk::createQuat(fmwk::Y, 90), glm::vec3(4,5,5), "wallDecor").addInstance();
    fmwk::Decoration(glm::vec3(36.1,0, -21), fmwk::createQuat(fmwk::Y, 90), glm::vec3(4,5,5), "wallDecor").addInstance();
    fmwk::Decoration(glm::vec3(27,0, -6.1), fmwk::createQuat(fmwk::Y, 0), glm::vec3(5,5,4), "wallDecor").addInstance();
    fmwk::Decoration(glm::vec3(59.9,0, -117), fmwk::createQuat(fmwk::Y, 90), glm::vec3(4,5,5), "wallDecor").addInstance();
    fmwk::Decoration(glm::vec3(9,0, -101.9), fmwk::createQuat(fmwk::Y, 0), glm::vec3(4,5,4), "wallDecor").addInstance();
    fmwk::Decoration(glm::vec3(77.9f,0, -93), fmwk::createQuat(fmwk::Y, 90), glm::vec3(4,5,5), "wallDecor").addInstance();
    fmwk::Decoration(glm::vec3(102.1f,0, -93), fmwk::createQuat(fmwk::Y, 90), glm::vec3(4, 5, 5), "wallDecor").addInstance();
    fmwk::Decoration(glm::vec3(102.1f,0, -99), fmwk::createQuat(fmwk::Y, 90), glm::vec3(4, 5, 5), "wallDecor").addInstance();

    fmwk::Decoration(glm::vec3(101.9f,2, -69), fmwk::createQuat(fmwk::Y, 90), glm::vec3(3), "wallChains").addInstance();
    fmwk::Decoration(glm::vec3(89.9f,2, -33), fmwk::createQuat(fmwk::Y, 90), glm::vec3(3), "wallChains").addInstance();
    fmwk::Decoration(glm::vec3(51,2, -53.9f), fmwk::createQuat(fmwk::Y, 0), glm::vec3(3), "wallChains").addInstance();
    fmwk::Decoration(glm::vec3(51,2, -18.1f), fmwk::createQuat(fmwk::Y, 0), glm::vec3(3), "wallChains").addInstance();
    fmwk::Decoration(glm::vec3(6.1f,2, -45), fmwk::createQuat(fmwk::Y, 90), glm::vec3(3), "wallChains").addInstance();
    fmwk::Decoration(glm::vec3(131.9f,2, -45), fmwk::createQuat(fmwk::Y, 90), glm::vec3(3), "wallChains").addInstance();
    fmwk::Decoration(glm::vec3(123,2, -77.9f), fmwk::createQuat(fmwk::Y, 0), glm::vec3(3), "wallChains").addInstance();
    fmwk::Decoration(glm::vec3(51,2, -83.9), fmwk::createQuat(fmwk::Y, 0), glm::vec3(3), "wallChains").addInstance();
    fmwk::Decoration(glm::vec3(29.9f,2, -111), fmwk::createQuat(fmwk::Y, 90), glm::vec3(3), "wallChains").addInstance();
    fmwk::Decoration(glm::vec3(17.9f,2, -81), fmwk::createQuat(fmwk::Y, 90), glm::vec3(3), "wallChains").addInstance();

    fmwk::Decoration(glm::vec3(108.1f,2, -33), fmwk::createQuat(fmwk::X, 90, fmwk::Z, -90), glm::vec3(3), "wallCircle").addInstance();
    fmwk::Decoration(glm::vec3(24.1f,2, -57), fmwk::createQuat(fmwk::X, 90, fmwk::Z, -90), glm::vec3(3), "wallCircle").addInstance();
    fmwk::Decoration(glm::vec3(72.1f,2, -21), fmwk::createQuat(fmwk::X, 90, fmwk::Z, -90), glm::vec3(3), "wallCircle").addInstance();
    fmwk::Decoration(glm::vec3(6.1f,2, -15), fmwk::createQuat(fmwk::X, 90, fmwk::Z, -90), glm::vec3(3), "wallCircle").addInstance();
    fmwk::Decoration(glm::vec3(54.1f,2, -105), fmwk::createQuat(fmwk::X, 90, fmwk::Z, -90), glm::vec3(3), "wallCircle").addInstance();
    fmwk::Decoration(glm::vec3(126.1f,2, -69), fmwk::createQuat(fmwk::X, 90, fmwk::Z, -90), glm::vec3(3), "wallCircle").addInstance();
    fmwk::Decoration(glm::vec3(72.1f,2, -69), fmwk::createQuat(fmwk::X, 90, fmwk::Z, -90), glm::vec3(3), "wallCircle").addInstance();

    fmwk::Decoration(glm::vec3(12.1f,2, -27), fmwk::createQuat(fmwk::Y, 90), glm::vec3(3), "wallShield").addInstance();
    fmwk::Decoration(glm::vec3(27,2, -119.9), fmwk::createQuat(fmwk::Y, 0), glm::vec3(3), "wallShield").addInstance();
    fmwk::Decoration(glm::vec3(63,2, -6.1f), fmwk::createQuat(fmwk::Y, 180), glm::vec3(3), "wallShield").addInstance();
    fmwk::Decoration(glm::vec3(75,2, -47.9), fmwk::createQuat(fmwk::Y, 0), glm::vec3(3), "wallShield").addInstance();
    fmwk::Decoration(glm::vec3(123,2, -12.1), fmwk::createQuat(fmwk::Y, 180), glm::vec3(3), "wallShield").addInstance();
    fmwk::Decoration(glm::vec3(93,2, -77.9), fmwk::createQuat(fmwk::Y, 0), glm::vec3(3), "wallShield").addInstance();
    fmwk::Decoration(glm::vec3(84.1f,2, -27), fmwk::createQuat(fmwk::Y, 90), glm::vec3(3), "wallShield").addInstance();

    fmwk::Decoration(glm::vec3(84.1f,2.5, -115), fmwk::createQuat(fmwk::Y, 90), glm::vec3(5), "shelf1").addInstance();
    fmwk::Decoration(glm::vec3(90,2.5, -119.9f), fmwk::createQuat(fmwk::Y, 0), glm::vec3(5), "shelf2").addInstance();
    fmwk::Decoration(glm::vec3(126,2.5, -84.1f), fmwk::createQuat(fmwk::Y, 180), glm::vec3(5), "shelf1").addInstance();
    fmwk::Decoration(glm::vec3(131.9f,2.5, -90), fmwk::createQuat(fmwk::Y, -90), glm::vec3(5), "shelf2").addInstance();

    fmwk::Decoration(glm::vec3(99,3, -47.9f), fmwk::createQuat(fmwk::X, 90), glm::vec3(3), "closedWindow").addInstance();
    fmwk::Decoration(glm::vec3(57,3, -36.9f), fmwk::createQuat(fmwk::X, 90), glm::vec3(3), "closedWindow").addInstance();
    fmwk::Decoration(glm::vec3(33,3, -90.1f), fmwk::createQuat(fmwk::Y, 180, fmwk::Z, 90, fmwk::X, 90), glm::vec3(3), "closedWindow").addInstance();
    fmwk::Decoration(glm::vec3(42.1f,3, -9), fmwk::createQuat(fmwk::Z, 180, fmwk::Y, -90), glm::vec3(3), "closedWindow").addInstance();




//    auto sphereEntity = std::make_unique<fmwk::Entity>("Sphere", glm::vec3(87.0f,2.7f, -46.1f),glm::quat(1, 0, 0, 0));
//    sphereEntity->getTransform().setScale(glm::vec3(0.5f));
//    sphereEntity->addComponent(std::make_unique<fmwk::MeshComponent>(gameEngine->getModelByName("mySphere")));
//    sphereEntity->addComponent(std::make_unique<fmwk::TextureComponent>(gameEngine->getBoundTextureByName("sphereTexture")));
//    sphereEntity->addComponent(std::make_unique<fmwk::DefaultMaterial>(100.0));
//    gameEngine->addEntity(std::move(sphereEntity));
//
//   fmwk::Decoration(glm::vec3(87.0f,2.7f, -46.1f), fmwk::createQuat(fmwk::Y, 270), glm::vec3(0.5), "mySphere").addInstance();
//    auto streetLight = std::make_unique<fmwk::Entity>("streetLight", glm::vec3(87.0f, 0.7f, -46.1f),
//                                                      glm::quat(1, 0, 0, 0));
//    streetLight->addComponent(std::make_unique<fmwk::PointLightComponent>("streetLight1", glm::vec4(1,1,1,1), 0.0f, 4));

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

    std::vector<glm::vec3> e18 = {
            glm::vec3(15, 0.5, -99),
            glm::vec3(15, 0.5, -117),
            glm::vec3(57, 0.5, -117)
    };

    std::vector<glm::vec3> e19 = {
            glm::vec3(27, 0.5, -63),
            glm::vec3(27, 0.5, -81),
            glm::vec3(63, 0.5, -81)
    };

    fmwk::PowerUpCube(glm::vec3(87.0f,0.5,-111.0f), fmwk::SET_SHIELD).addInstance();
    fmwk::PowerUpCube(glm::vec3(123.0f,0.5,-99.0f), fmwk::SET_SHIELD).addInstance();
    fmwk::PowerUpCube(glm::vec3(111.0f,0.5,-21.0f), fmwk::SET_SHIELD).addInstance();
    fmwk::PowerUpCube(glm::vec3(9.0f,0.5,-9.0f), fmwk::SET_SHIELD).addInstance();
    fmwk::PowerUpCube(glm::vec3(75.0f,0.5,-9.0f), fmwk::SET_SHIELD).addInstance();
    fmwk::PowerUpCube(glm::vec3(85.0f,0.5, -18.0f), fmwk::SET_SHIELD).addInstance();
    fmwk::PowerUpCube(glm::vec3(87.0f,0.5,-69.0f), fmwk::SET_SHIELD).addInstance();
    fmwk::PowerUpCube(glm::vec3(9.0f,0.5,-111.0f), fmwk::SET_SHIELD).addInstance();

    fmwk::PowerUpCube(glm::vec3(87.0f,0.5,-87.0f), fmwk::ADD_LIFE).addInstance();
    fmwk::PowerUpCube(glm::vec3(39.0f,0.5,-81.0f), fmwk::ADD_LIFE).addInstance();
    fmwk::PowerUpCube(glm::vec3(51.0f,0.5,-39.0f), fmwk::ADD_LIFE).addInstance();

    fmwk::PowerUpCube(glm::vec3(108.0f,0.5,-102.0f), fmwk::DECREASE_BULLET_COOL_DOWN).addInstance();

    fmwk::PowerUpCube(glm::vec3(27.0f,0.5,-27.0f), fmwk::INCREASE_SPEED).addInstance();

    fmwk::PowerUpCube(glm::vec3(81.0f,0.5, -87.0f), fmwk::SPAWN_BOSS_ENEMY).addInstance();

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
    fmwk::BasicEnemy(e18).addInstance();
    fmwk::BasicEnemy(e19).addInstance();



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
