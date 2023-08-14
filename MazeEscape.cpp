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
    initialBackgroundColor = {0.0f, 0.005f, 0.01f, 1.0f};

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
    gameEngine->addModel("mySphere", fmwk::VERTEX_WITH_NORMAL_AND_TANGENT, "models/Sphere.gltf");
    gameEngine->addTexture("cubeTexture", "textures/Checker.png");
    gameEngine->addTexture("sphereTexture", "textures/Metals_09_basecolor.png");
    gameEngine->addTexture("sphereNormal", "textures/Metals_09_normal.png");
    gameEngine->addTexture("sphereMaterial", "textures/Metals_09_met_rough_ao.png");
    gameEngine->addTexture("mainMazeTexture", "textures/maze_main.png");

    auto [vertices, faces] = buildMazeModel();
    gameEngine->addModel<fmwk::VertexWithNormal>("MazeModel", fmwk::VERTEX_WITH_NORMAL, vertices, faces);

    auto cameraEntity = std::make_unique<fmwk::Entity>("Camera", glm::vec3(0, 0, 10), glm::quat(1, 0, 0, 0));
    auto cameraComponent = std::make_unique<fmwk::PerspectiveCamera>(0.1f, 100.0f, glm::radians(45.0f));
    cameraEntity->addComponent(std::move(cameraComponent));

    auto characterEntity = std::make_unique<fmwk::Entity>("Character", glm::vec3(1.5f,0.5f, 1.5f), glm::quat(1,0,0,0));
    characterEntity->getTransform().setScale({0.5, 0.5, 0.5});

    characterEntity->addComponent(std::make_unique<fmwk::MeshComponent>(gameEngine->getModelByName("myCube")));
    characterEntity->addComponent(std::make_unique<fmwk::Collider>(1.0f, "CHARACTER", glm::vec3(0,0,0)));
    characterEntity->addComponent(std::make_unique<fmwk::TextureComponent>(gameEngine->getBoundTextureByName("cubeTexture")));
    characterEntity->addComponent(std::make_unique<fmwk::SimplePhongMaterial>());
    std::unordered_set<std::string> characterBulletTags = std::unordered_set<std::string>{"ENEMY"};
    characterEntity->addComponent(std::make_unique<fmwk::CharacterBulletSpawner>(glm::vec3(0,0,0), 8.5f, 1.0f, &characterBulletTags));
    characterEntity->addComponent(std::make_unique<fmwk::CharacterController>("CharacterController", cameraEntity->getTransform(), 5.0f));
    cameraEntity->addComponent(std::make_unique<fmwk::CameraController>("CameraController", characterEntity->getTransform(), glm::radians(120.0f), 3.0f, 0.25f));


    auto mazeEntity = std::make_unique<fmwk::Entity>("Maze");
    mazeEntity->addComponent(std::make_unique<fmwk::MeshComponent>(gameEngine->getModelByName("MazeModel")));
    mazeEntity->addComponent(std::make_unique<fmwk::TextureComponent>(gameEngine->getBoundTextureByName("mainMazeTexture")));
    mazeEntity->addComponent(std::make_unique<fmwk::SimplePhongMaterial>());


    std::vector<glm::vec3> enemyTargetPoints = {6.0f * glm::vec3(7.5, 0.5/6.0f, -3.5), 6.0f * glm::vec3(7.5, 0.5/6.0f, -1.5), 6.0f * glm::vec3(1.5, 0.5/6.0f, -1.5), 6.0f * glm::vec3(1.5, 0.5/6.0f, -4.5)};

    auto enemyEntity = std::make_unique<fmwk::Entity>("Enemy", enemyTargetPoints[0], glm::quat(1,0,0,0));
    //enemyEntity->getTransform().setScale({0.5, 0.5, 0.5});
    enemyEntity->addComponent(std::make_unique<fmwk::Collider>(1.0f, "ENEMY", glm::vec3(0,0,0)));
    enemyEntity->addComponent(std::make_unique<fmwk::MeshComponent>(gameEngine->getModelByName("mySphere")));
    enemyEntity->addComponent(std::make_unique<fmwk::TextureComponent>(gameEngine->getBoundTextureByName("sphereTexture")));
    enemyEntity->addComponent(std::make_unique<fmwk::DefaultMaterial>(1.0f));
    enemyEntity->addComponent(std::make_unique<fmwk::EnemyController>(enemyTargetPoints, 2.0f, 6.0f, 1.0f));
    enemyEntity->addComponent(std::make_unique<fmwk::EnemyCollisionBehaviour>());


    auto lightEntity = std::make_unique<fmwk::Entity>("LightEntity", glm::vec3(0,3,0), glm::rotate(glm::quat(1,0,0,0), glm::radians(-90.0f), fmwk::X));
    lightEntity->addComponent(std::make_unique<fmwk::DirectLightComponent>("DirectLight", glm::normalize(glm::vec3(-1, -1, 1)), glm::vec4(1)));
    gameEngine->addEntity(std::move(cameraEntity));
    gameEngine->addEntity(std::move(characterEntity));
    gameEngine->addEntity(std::move(mazeEntity));
    gameEngine->addEntity(std::move(enemyEntity));
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

std::pair<std::vector<fmwk::VertexWithNormal>, std::vector<uint32_t>> MazeEscape::buildMazeModel() {
    char *mazeIn[] = {

            (char*)("### ###########"),
            (char*)("# #   #       #"),
            (char*)("# # ### ### ###"),
            (char*)("#   #     #   #"),
            (char*)("# ### ####### #"),
            (char*)("#   # #       #"),
            (char*)("### ### #######"),
            (char*)("# #   #       #"),
            (char*)("# ### ####### #"),
            (char*)("# #   #   #   #"),
            (char*)("# # ### # # # #"),
            (char*)("#   #   # # # #"),
            (char*)("# ##### # # # #"),
            (char*)("#       #   # #"),
            (char*)("# #############")
    };
    int row = 15;
    int col = 15;
    auto vec = std::vector<std::vector<char>>(row, std::vector<char>(col, '#'));
    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            vec[i][j] = mazeIn[row-1-i][j];
        }
    }

    mgen::Maze myMaze = mgen::Maze(vec, row, col, 6.0f);
    myMaze.buildBoxes();
    myMaze.deleteUselessFaces();
    auto vertexDict = mgen::VertexDictionary();
    for(auto const& box : myMaze.getBoxes()){
        for(auto& r : box->buildRectangles()){
            vertexDict.addRectangle(r);
        }
    }

    std::vector<fmwk::VertexWithNormal> vertexes{};
    std::vector<uint32_t> indices{};

    for(auto& vertex : vertexDict.getVertices()){
        vertexes.push_back({vertex.pos, vertex.UV, vertex.norm});
    }

    for(auto& triangle : vertexDict.getTriangles()){
        for(int i = 0; i < 3; i++)
            indices.push_back(triangle[i]);
    }

    return {vertexes, indices};



}
