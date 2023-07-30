// This has been adapted from the Vulkan tutorial

#include "Starter.hpp"
#include "framework/GameEngine.h"
#include "framework/components/mesh/MeshComponent.h"
#include "framework/components/texture/TextureComponent.h"
#include "framework/components/materials/DefaultMaterial.h"
#include "framework/components/camera/PerspectiveCamera.h"
#include "framework/components/scripts/CharacterController.h"
#include "framework/components/scripts/CameraController.h"
#include "framework/components/materials/ColorBlendComponent.h"
#include "framework/components/scripts/CubeSpawner.h"
#include "framework/components/materials/GGXMaterial.h"
#include "framework/components/lights/DirectLightComponent.h"
#include "framework/components/lights/PointLightComponent.h"
#include "framework/components/lights/SpotLightComponent.h"


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






// MAIN ! 
class SimpleCube : public BaseProject {
	protected:

	// Current aspect ratio (used by the callback that resized the window
	float Ar;


	// Other application parameters

	// Here you set the main application parameters
	void setWindowParameters() {
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
	void onWindowResize(int w, int h) {
		Ar = (float)w / (float)h;
	}
	
	// Here you load and setup all your Vulkan Models and Texutures.
	// Here you also create your Descriptor set layouts and load the shaders for the pipelines
	void localInit() {

        auto gameEngine = fmwk::GameEngine::getInstance();
        gameEngine->addModel("myCube", fmwk::VERTEX_WITH_NORMAL, "Models/Cube.obj");
        gameEngine->addModel("mySphere", fmwk::VERTEX_WITH_NORMAL_AND_TANGENT, "models/Sphere.gltf");
        gameEngine->addTexture("cubeTexture", "textures/Checker.png");

        gameEngine->addTexture("sphereTexture", "textures/Metals_09_basecolor.png");
        gameEngine->addTexture("sphereNormal", "textures/Metals_09_normal.png");
        gameEngine->addTexture("sphereMaterial", "textures/Metals_09_met_rough_ao.png");


        auto cubeEntity = std::make_unique<fmwk::Entity>("myCubeEntity");
        auto modelComponent = std::make_unique<fmwk::MeshComponent>(gameEngine->getModelByName("myCube"));
        auto textureComponent = std::make_unique<fmwk::TextureComponent>(gameEngine->getBoundTextureByName("cubeTexture"));
        auto materialComponent = std::make_unique<fmwk::DefaultMaterial>(1.5f);
        auto cubeSpawner = std::make_unique<fmwk::CubeSpawner>("CubeSpawner");
        cubeEntity->addComponent(std::move(modelComponent));
        cubeEntity->addComponent(std::move(textureComponent));
        cubeEntity->addComponent(std::move(materialComponent));
        cubeEntity->addComponent(std::move(cubeSpawner));


        auto camera = std::make_unique<fmwk::Entity>("Camera", glm::vec3(0,0,8), glm::quat());
        auto perspectiveCameraComponent = std::make_unique<fmwk::PerspectiveCamera>(0.1f, 100.0f, glm::radians(45.0f));
        camera->addComponent(std::move(perspectiveCameraComponent));

        auto characterController = std::make_unique<fmwk::CharacterController>("CharacterController", camera->getTransform(), 4.0f);
        auto cameraController = std::make_unique<fmwk::CameraController>("CameraController", cubeEntity->getTransform(), glm::radians(120.0f), 8.0f, 0.25f);

        cubeEntity->addComponent(std::move(characterController));
        camera->addComponent(std::move(cameraController));

        gameEngine->addEntity(std::move(cubeEntity));
        gameEngine->addEntity(std::move(camera));

        auto directLight = std::make_unique<fmwk::Entity>("DirectLight");
        auto directLightComponent = std::make_unique<fmwk::DirectLightComponent>("DirectLight", glm::vec3(1, 0, 0), glm::vec4(1,1,1,1));
        //directLightComponent->turnOff();
        directLight->addComponent(std::move(directLightComponent));
        gameEngine->addEntity(std::move(directLight));

        auto pointLight = std::make_unique<fmwk::Entity>("PointLight");
        auto pointLightComponent = std::make_unique<fmwk::PointLightComponent>("PointLightComponent", glm::vec4(1,1,1,1), 2.0f, 1.5f);
       // pointLightComponent->turnOff();
        pointLight->addComponent(std::move(pointLightComponent));
        gameEngine->addEntity(std::move(pointLight));

        auto spotLight = std::make_unique<fmwk::Entity>("SpotLight", glm::vec3(0, 4, 0), glm::vec3(glm::radians(-90.0f), 0, 0));
        auto spotLightComponent = std::make_unique<fmwk::SpotLightComponent>("SpotLightComponent", glm::vec4(1,1,1,1), 2.0f, 1.5f, 0.85f, 0.95f);
        //spotLightComponent->turnOff();
        spotLight->addComponent(std::move(spotLightComponent));
        gameEngine->addEntity(std::move(spotLight));

		
		// Init local variables
        gameEngine->provisionResources(false);
	}
	
	// Here you create your pipelines and Descriptor Sets!
	void pipelinesAndDescriptorSetsInit() {
        auto gameEngine = fmwk::GameEngine::getInstance();
        gameEngine->rebuildResources();

	}

	// Here you destroy your pipelines and Descriptor Sets!
	// All the object classes defined in Starter.hpp have a method .cleanup() for this purpose
	void pipelinesAndDescriptorSetsCleanup() {
        std::cout << "PIPELINES AND DESCRIPTOR SETS CLEANUP CALLED" << std::endl;
        auto gameEngine = fmwk::GameEngine::getInstance();
        gameEngine->cleanupResources();

	}

	// Here you destroy all the Models, Texture and Desc. Set Layouts you created!
	// All the object classes defined in Starter.hpp have a method .cleanup() for this purpose
	// You also have to destroy the pipelines: since they need to be rebuilt, they have two different
	// methods: .cleanup() recreates them, while .destroy() delete them completely
	void localCleanup() {
        std::cout << "LOCAL CLEANUP CALLED" << std::endl;

	}
	
	// Here it is the creation of the command buffer:
	// You send to the GPU all the objects you want to draw,
	// with their buffers and textures

	void populateCommandBuffer(VkCommandBuffer commandBuffer, int currentImage) {
        auto gameEngine = fmwk::GameEngine::getInstance();
        gameEngine->renderFrame(commandBuffer, currentImage);
	}

	// Here is where you update the uniforms.
	// Very likely this will be where you will be writing the logic of your application.
	void updateUniformBuffer(uint32_t currentImage) {
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
};


// This is the main: probably you do not need to touch this!
int main() {
    SimpleCube app;

    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}