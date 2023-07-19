// This has been adapted from the Vulkan tutorial

#include "Starter.hpp"
#include "framework/GameEngine.h"
#include "framework/MeshComponent.h"

// The uniform buffer objects data structures
// Remember to use the correct alignas(...) value
//        float : alignas(4)
//        vec2  : alignas(8)
//        vec3  : alignas(16)
//        vec4  : alignas(16)
//        mat3  : alignas(16)
//        mat4  : alignas(16)

struct GlobalUniformBlock{
    alignas(16) glm::mat4 vpMat;
};

struct EntityTransformUniformBlock{
    alignas(16) glm::mat4 mMat;
};

// The vertices data structures
// Example
struct Vertex {
	glm::vec3 pos;
	glm::vec2 UV;
};





// MAIN ! 
class SimpleCube : public BaseProject {
	protected:

	// Current aspect ratio (used by the callback that resized the window
	float Ar;

	// Descriptor Layouts ["classes" of what will be passed to the shaders]
	DescriptorSetLayout globalSetLayout, transformSetLayout;

	// Pipelines [Shader couples]
	Pipeline P;


	// Descriptor sets
	DescriptorSet globalSet, cubeTransformSet;
	// Textures

	
	// C++ storage for uniform variables
	GlobalUniformBlock gubo;
    EntityTransformUniformBlock tubo;

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
		// Descriptor Layouts [what will be passed to the shaders]
		globalSetLayout.init(this, {
					// this array contains the bindings:
					// first  element : the binding number
					// second element : the type of element (buffer or texture)
					//                  using the corresponding Vulkan constant
					// third  element : the pipeline stage where it will be used
					//                  using the corresponding Vulkan constant
					{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT}
				});

        transformSetLayout.init(this, {
                // this array contains the bindings:
                // first  element : the binding number
                // second element : the type of element (buffer or texture)
                //                  using the corresponding Vulkan constant
                // third  element : the pipeline stage where it will be used
                //                  using the corresponding Vulkan constant
                {0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT}
        });


		// Pipelines [Shader couples]
		// The second parameter is the pointer to the vertex definition
		// Third and fourth parameters are respectively the vertex and fragment shaders
		// The last array, is a vector of pointer to the layouts of the sets that will
		// be used in this pipeline. The first element will be set 0, and so on..
		P.init(this, &gameEngine->getAllVertexDescriptors().find(fmwk::VERTEX)->second, "shaders/ShaderVert.spv", "shaders/ShaderFrag.spv", {&globalSetLayout, &gameEngine->getTextureDescriptorSetLayout(), &transformSetLayout});

		// Models, textures and Descriptors (values assigned to the uniforms)

		// Create models
		// The second parameter is the pointer to the vertex definition for this model
		// The third parameter is the file name
		// The last is a constant specifying the file type: currently only OBJ or GLTF
        gameEngine->addModel("myCube", fmwk::VERTEX, "Models/Cube.obj");
		//cubeModel.init(this, &simpleVertexDescriptor, "Models/Cube.obj", OBJ);

        auto cubeEntity = std::make_unique<fmwk::Entity>("myCubeEntity");
        auto modelComponent = std::make_unique<fmwk::MeshComponent>("Mesh", gameEngine->getModelByName("myCube"));
        cubeEntity->addComponent(std::move(modelComponent));
        gameEngine->addEntity(std::move(cubeEntity));


		// Create the textures
		// The second parameter is the file name
        gameEngine->addTexture("cubeTexture", "textures/Checker.png");
		//cubeTexture.init(this,"textures/Checker.png");
		
		// Init local variables
	}
	
	// Here you create your pipelines and Descriptor Sets!
	void pipelinesAndDescriptorSetsInit() {
		// This creates a new pipeline (with the current surface), using its shaders
		P.create();

		// Here you define the data set
		globalSet.init(this, &globalSetLayout, {
		// the second parameter, is a pointer to the Uniform Set Layout of this set
		// the last parameter is an array, with one element per binding of the set.
		// first  elmenet : the binding number
		// second element : UNIFORM or TEXTURE (an enum) depending on the type
		// third  element : only for UNIFORMs, the size of the corresponding C++ object. For texture, just put 0
		// fourth element : only for TEXTUREs, the pointer to the corresponding texture object. For uniforms, use nullptr
					{0, UNIFORM, sizeof(GlobalUniformBlock)}
        });
        cubeTransformSet.init(this, &transformSetLayout, {
                {0, UNIFORM, sizeof(EntityTransformUniformBlock)}
        });
	}

	// Here you destroy your pipelines and Descriptor Sets!
	// All the object classes defined in Starter.hpp have a method .cleanup() for this purpose
	void pipelinesAndDescriptorSetsCleanup() {
		// Cleanup pipelines
		P.cleanup();

		// Cleanup datasets
		globalSet.cleanup();
        fmwk::GameEngine::getInstance()->getBoundTextureByName("cubeTexture").textureSet.cleanup();
        cubeTransformSet.cleanup();
	}

	// Here you destroy all the Models, Texture and Desc. Set Layouts you created!
	// All the object classes defined in Starter.hpp have a method .cleanup() for this purpose
	// You also have to destroy the pipelines: since they need to be rebuilt, they have two different
	// methods: .cleanup() recreates them, while .destroy() delete them completely
	void localCleanup() {
        auto gameEngine = fmwk::GameEngine::getInstance();
		// Cleanup textures
		gameEngine->getBoundTextureByName("cubeTexture").texture.cleanup();
		
		// Cleanup models
		//baseModel->cleanup();
        fmwk::GameEngine::getInstance()->getModelByName("myCube").cleanup();

		// Cleanup descriptor set layouts
		globalSetLayout.cleanup();
        gameEngine->getTextureDescriptorSetLayout().cleanup();
        transformSetLayout.cleanup();

		
		// Destroies the pipelines
		P.destroy();		
	}
	
	// Here it is the creation of the command buffer:
	// You send to the GPU all the objects you want to draw,
	// with their buffers and textures
	
	void populateCommandBuffer(VkCommandBuffer commandBuffer, int currentImage) {
        auto gameEngine = fmwk::GameEngine::getInstance();
		// binds the pipeline
		P.bind(commandBuffer);
		// For a pipeline object, this command binds the corresponing pipeline to the command buffer passed in its parameter

		// binds the data set
		globalSet.bind(commandBuffer, P, 0, currentImage);
        gameEngine->getBoundTextureByName("cubeTexture").textureSet.bind(commandBuffer, P, 1, currentImage);
        cubeTransformSet.bind(commandBuffer, P, 2, currentImage);

		// For a Dataset object, this command binds the corresponing dataset
		// to the command buffer and pipeline passed in its first and second parameters.
		// The third parameter is the number of the set being bound
		// As described in the Vulkan tutorial, a different dataset is required for each image in the swap chain.
		// This is done automatically in file Starter.hpp, however the command here needs also the index
		// of the current image in the swap chain, passed in its last parameter
					
		// binds the model
		//baseModel->bind(commandBuffer);
        dynamic_cast<fmwk::MeshComponent&>(gameEngine->getEntityByName("myCubeEntity").getComponentByName("Mesh")).getModel().bind(commandBuffer);
        //fmwk::GameEngine::getInstance()->getModelByName("myCube").bind(commandBuffer);

		// For a Model object, this command binds the corresponing index and vertex buffer
		// to the command buffer passed in its parameter
		
		// record the drawing command in the command buffer
		vkCmdDrawIndexed(commandBuffer,
                         fmwk::GameEngine::getInstance()->getModelByName("myCube").getVertexCount(), 1, 0, 0, 0);
		// the second parameter is the number of indexes to be drawn. For a Model object,
		// this can be retrieved with the .indices.size() method.
	}

	// Here is where you update the uniforms.
	// Very likely this will be where you will be writing the logic of your application.
	void updateUniformBuffer(uint32_t currentImage) {
		// Standard procedure to quit when the ESC key is pressed
		if(glfwGetKey(window, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		
		// Integration with the timers and the controllers
		float deltaT;
		auto m = glm::vec3(0.0f), r = glm::vec3(0.0f);
		bool fire = false;
		getSixAxis(deltaT, m, r, fire);
		// getSixAxis() is defined in Starter.hpp in the base class.
		// It fills the float point variable passed in its first parameter with the time
		// since the last call to the procedure.
		// It fills vec3 in the second parameters, with three values in the -1,1 range corresponding
		// to motion (with left stick of the gamepad, or ASWD + RF keys on the keyboard)
		// It fills vec3 in the third parameters, with three values in the -1,1 range corresponding
		// to motion (with right stick of the gamepad, or Arrow keys + QE keys on the keyboard, or mouse)
		// If fills the last boolean variable with true if fire has been pressed:
		//          SPACE on the keyboard, A or B button on the Gamepad, Right mouse button

		
		// Parameters
		// Camera FOV-y, Near Plane and Far Plane
		const float FOVy = glm::radians(90.0f);
		const float nearPlane = 0.1f;
		const float farPlane = 100.0f;
		
		glm::mat4 Prj = glm::perspective(FOVy, Ar, nearPlane, farPlane);
		Prj[1][1] *= -1;
		glm::vec3 camTarget = glm::vec3(0,0,0);
		glm::vec3 camPos    = camTarget + glm::vec3(6,3,10);
		glm::mat4 View = glm::lookAt(camPos, camTarget, glm::vec3(0,1,0));


		glm::mat4 World = glm::mat4(1);		
		gubo.vpMat = Prj * View;
        tubo.mMat = World;
		globalSet.map(currentImage, &gubo, sizeof(gubo), 0);
        cubeTransformSet.map(currentImage, &tubo, sizeof(tubo), 0);
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