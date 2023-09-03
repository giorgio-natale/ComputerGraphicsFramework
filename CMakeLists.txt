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
        framework/components/scripts/MovingCubeController.h framework/components/collision/Collider.cpp framework/components/collision/Collider.h framework/systems/CollisionSystem.cpp framework/systems/CollisionSystem.h framework/components/scripts/EntityDespawner.cpp framework/components/scripts/EntityDespawner.h
        demo/ColliderTest.cpp
        demo/ColliderTest.h
        framework/components/scripts/TestCollisionChecker.cpp
        framework/components/scripts/TestCollisionChecker.h
        framework/components/scripts/BulletSpawner.cpp
        framework/components/scripts/BulletSpawner.h
        framework/components/scripts/BulletAI.cpp
        framework/components/scripts/BulletAI.h
        demo/BulletSpawnerTest.cpp
        demo/BulletSpawnerTest.h
        demo/MazeGeneration.cpp
        demo/MazeGeneration.h
        maze_generation/Direction.cpp
        maze_generation/Direction.h
        maze_generation/Position.cpp
        maze_generation/Position.h
        maze_generation/mazeUtils.h
        maze_generation/FragmentedFace.cpp
        maze_generation/FragmentedFace.h
        maze_generation/FragmentedBox.cpp
        maze_generation/FragmentedBox.h
        maze_generation/VertexDictionary.cpp
        maze_generation/VertexDictionary.h
        maze_generation/MazeCell.cpp
        maze_generation/MazeCell.h
        maze_generation/Maze.cpp
        maze_generation/Maze.h
        framework/components/scripts/MazeRepresentation.cpp
        framework/components/scripts/MazeRepresentation.h
        framework/components/scripts/EnemyCollisionBehaviour.cpp
        framework/components/scripts/EnemyCollisionBehaviour.h
        demo/EnemyDemo.cpp
        demo/EnemyDemo.h
        framework/components/scripts/EnemyController.cpp
        framework/components/scripts/EnemyController.h
        MazeEscape.cpp
        MazeEscape.h
        framework/components/scripts/MazeCharacterController.cpp
        framework/components/scripts/MazeCharacterController.h
        framework/components/scripts/CharacterBulletSpawner.cpp
        framework/components/scripts/CharacterBulletSpawner.h
        framework/components/scripts/EnemyBulletSpawner.cpp
        framework/components/scripts/EnemyBulletSpawner.h
        demo/ModelTransformationAndMaterialDemo.cpp
        demo/ModelTransformationAndMaterialDemo.h
        framework/components/materials/SimplePhongBlinkMaterial.cpp
        framework/components/materials/SimplePhongBlinkMaterial.h
        framework/components/materials/DefaultTransparent.cpp
        framework/components/materials/DefaultTransparent.h
        framework/components/materials/SimplePhongColorBlendMaterial.cpp
        framework/components/materials/SimplePhongColorBlendMaterial.h
        framework/components/scripts/Health.cpp
        framework/components/scripts/Health.h framework/components/sprite/Sprite.cpp framework/components/sprite/Sprite.h framework/components/materials/DefaultSprite.cpp framework/components/materials/DefaultSprite.h demo/HudDemo.cpp demo/HudDemo.h framework/components/sprite/SpriteUtils.cpp framework/components/sprite/SpriteUtils.h framework/blueprints/Blueprint.cpp demo/BlueprintDemo.cpp demo/BlueprintDemo.h framework/blueprints/Character.cpp framework/blueprints/Character.h framework/blueprints/RoughSphere.cpp framework/blueprints/RoughSphere.h framework/blueprints/BlockMaze.cpp framework/blueprints/BlockMaze.h framework/blueprints/BasicEnemy.cpp framework/blueprints/BasicEnemy.h framework/blueprints/HeartUI.cpp framework/blueprints/HeartUI.h framework/components/scripts/HeartController.cpp framework/components/scripts/HeartController.h framework/blueprints/Bullet.cpp framework/blueprints/Bullet.h framework/blueprints/BossEnemy.cpp framework/blueprints/BossEnemy.h framework/components/scripts/FinalBossController.cpp framework/blueprints/BossBarUI.cpp framework/blueprints/BossBarUI.h framework/components/scripts/BossBarController.cpp framework/components/scripts/BossBarController.h
        framework/blueprints/PowerUpCube.cpp
        framework/blueprints/PowerUpCube.h
        framework/components/scripts/TriggerPowerUp.cpp
        framework/components/scripts/TriggerPowerUp.h
        framework/components/scripts/PowerUp.cpp
        framework/components/scripts/PowerUp.h
        framework/components/scripts/PowerUpAddLife.cpp
        framework/components/scripts/PowerUpAddLife.h
        framework/components/scripts/PowerUpSetShield.cpp
        framework/components/scripts/PowerUpSetShield.h
        framework/components/scripts/PowerUpIncreaseSpeed.cpp
        framework/components/scripts/PowerUpIncreaseSpeed.h
        framework/components/scripts/PowerUpDecreaseBulletCoolDown.cpp
        framework/components/scripts/PowerUpDecreaseBulletCoolDown.h
        framework/components/scripts/PowerUpSpawnBossEnemy.cpp
        framework/components/scripts/PowerUpSpawnBossEnemy.h
        framework/blueprints/Shield.cpp
        framework/blueprints/Shield.h
        framework/components/scripts/FollowEntity.cpp
        framework/components/scripts/FollowEntity.h
        framework/blueprints/Decoration.cpp framework/blueprints/Decoration.h
        framework/components/scripts/Despawner.cpp
        framework/components/scripts/Despawner.h
        framework/components/scripts/CharacterDespawner.cpp
        framework/components/scripts/CharacterDespawner.h
        framework/components/scripts/BossEnemyDespawner.cpp
        framework/components/scripts/BossEnemyDespawner.h
        framework/components/scripts/ShieldDespawner.cpp
        framework/components/scripts/ShieldDespawner.h
        framework/components/scripts/DefaultDespawner.cpp
        framework/components/scripts/DefaultDespawner.h framework/blueprints/SplashScreen.cpp framework/blueprints/SplashScreen.h framework/blueprints/Floor.cpp framework/blueprints/Floor.h framework/blueprints/PowerupIcon.cpp framework/blueprints/PowerupIcon.h framework/components/materials/GGXNoNormalMaterial.cpp framework/components/materials/GGXNoNormalMaterial.h)
target_link_libraries(${PROJECT_NAME})
find_package(Vulkan REQUIRED)
foreach(dir IN LISTS Vulkan_INCLUDE_DIR INCLUDE_DIRS)
    target_include_directories(${PROJECT_NAME} PUBLIC ${dir})
endforeach()
foreach(lib IN LISTS Vulkan_LIBRARIES LINK_LIBS)
    target_link_libraries(${PROJECT_NAME} ${lib})
endforeach()
