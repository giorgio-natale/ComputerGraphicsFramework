#########################################################
# Variables #
# Set them to match your SDK and libraries installation #
#########################################################
file(READ "${CMAKE_CURRENT_LIST_DIR}/config.json" config_json)
string(JSON my_vulkan_sdk GET "${config_json}" vulkan sdk)
string(JSON my_glfw_base_path GET "${config_json}" glfw basePath)
string(JSON my_glfw_libs_sub_path GET "${config_json}" glfw libsSubPath)
string(JSON my_glm_libs GET "${config_json}" glm libs)


set($ENV{VULKAN_SDK} "${my_vulkan_sdk}") # Vulkan SDK
set(GLFW "${my_glfw_base_path}") # GLFW
set(GLM "${my_glm_libs}") # GLM
# List of directories to add to the include path:
# all headers in these directories can be included in the source code
# (Vulkan headers must be omitted from here)
list(APPEND INCLUDE_DIRS "${GLFW}/include" ${GLM} headers)
# List of libraries to link to the executable:
# all these libraries will be linked to the compiled executable
# (Vulkan library must be omitted from here)
list(APPEND LINK_LIBS "${GLFW}/${my_glfw_libs_sub_path}")

# Name of the project, will be used as the target _name for CMake build
# and will be the _name of the executable
set(PROJ_NAME Demo)
# Name of the source file containing the main() function
set(MAIN main.cpp)
#########################################################
# CMake configuration #
# You shouldn't need to modify anything past this point #
#########################################################
cmake_minimum_required(VERSION 3.24)
project(${PROJ_NAME} LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
add_executable(${PROJECT_NAME} ${MAIN} starter.cpp framework/components/scripts/Component.cpp framework/components/scripts/Component.h framework/Entity.cpp framework/Entity.h framework/components/camera/Camera.cpp framework/components/camera/Camera.h framework/components/transform/Transform.cpp framework/components/transform/Transform.h framework/utils.cpp framework/utils.h framework/GameEngine.cpp framework/GameEngine.h framework/components/camera/PerspectiveCamera.cpp framework/components/camera/PerspectiveCamera.h framework/components/scripts/CharacterController.cpp framework/components/scripts/CharacterController.h framework/components/scripts/CameraController.cpp framework/components/scripts/CameraController.h framework/systems/ModelSystem.cpp framework/systems/ModelSystem.h framework/GameEngineApi.h framework/components/mesh/MeshComponent.cpp framework/components/mesh/MeshComponent.h framework/systems/TextureSystem.cpp framework/systems/TextureSystem.h framework/components/texture/TextureComponent.cpp framework/components/texture/TextureComponent.h framework/systems/MaterialSystem.cpp framework/systems/MaterialSystem.h framework/systems/RenderSystem.cpp framework/systems/RenderSystem.h framework/components/materials/MaterialComponent.cpp framework/components/materials/MaterialComponent.h framework/components/materials/DefaultMaterial.cpp framework/components/materials/DefaultMaterial.h framework/systems/InputSystem.cpp framework/systems/InputSystem.h framework/components/materials/ColorBlendComponent.cpp framework/components/materials/ColorBlendComponent.h framework/components/scripts/CubeSpawner.cpp framework/components/scripts/CubeSpawner.h framework/components/materials/SimplePhongMaterial.cpp framework/components/materials/SimplePhongMaterial.h framework/components/materials/GGXMaterial.cpp framework/components/materials/GGXMaterial.h framework/components/lights/LightComponent.cpp framework/components/lights/LightComponent.h framework/components/lights/DirectLightComponent.cpp framework/components/lights/DirectLightComponent.h framework/components/lights/PointLightComponent.cpp framework/components/lights/PointLightComponent.h framework/components/lights/SpotLightComponent.cpp framework/components/lights/SpotLightComponent.h main.cpp demo/SimpleCube.h demo/SimpleCube.cpp
demo/MovingCube.cpp
        demo/MovingCube.h
        framework/components/scripts/MovingCubeController.cpp
        framework/components/scripts/MovingCubeController.h framework/components/collision/Collider.cpp framework/components/collision/Collider.h framework/systems/CollisionSystem.cpp framework/systems/CollisionSystem.h framework/components/scripts/EntityDespawner.cpp framework/components/scripts/EntityDespawner.h)
target_link_libraries(${PROJECT_NAME})
find_package(Vulkan REQUIRED)
foreach(dir IN LISTS Vulkan_INCLUDE_DIR INCLUDE_DIRS)
    target_include_directories(${PROJECT_NAME} PUBLIC ${dir})
endforeach()
foreach(lib IN LISTS Vulkan_LIBRARIES LINK_LIBS)
    target_link_libraries(${PROJECT_NAME} ${lib})
endforeach()
