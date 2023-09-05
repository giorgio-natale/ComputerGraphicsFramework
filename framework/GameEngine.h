#ifndef A07_GAMEENGINE_H
#define A07_GAMEENGINE_H

#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <chrono>
#include <utility>
#include <map>
#include <string>
#include <stdexcept>
#include <algorithm>
#include "Entity.h"
#include "GameEngineApi.h"
#include "../Starter.hpp"
#include "systems/ModelSystem.h"
#include "systems/TextureSystem.h"
#include "systems/MaterialSystem.h"
#include "systems/RenderSystem.h"
#include "systems/InputSystem.h"
#include "systems/CollisionSystem.h"
#include "components/materials/MaterialComponent.h"
//#include "systems/ModelSystem.h"

namespace fmwk {

    struct DescriptorSetInitializationInfo {
        DescriptorSetLayout *descriptorSetLayout;
        DescriptorSetClaim descriptorSetClaim;
    };

    class GameEngine : GameEngineApi {
    public:
        GameEngine(GameEngine &other) = delete;

        void operator=(const GameEngine &) = delete;

        static GameEngine *getInstance();

        static void createInstance(BaseProject *bp);

        void setWindow(GLFWwindow *window) override {
            _window = window;
        }

        void bootTextureSystem();

        void bootMaterialSystem();

        void bootRenderSystem();

        void bootInputSystem();

        void addEntity(std::unique_ptr<Entity> entity) override;

        void enqueueEntity(std::unique_ptr<Entity> entity);

        void removeEntity(std::string const &name);

        void enqueueEntityRemoval(std::string const &name);

        void enqueueRemovalOfAllEntities();

        void flushEnqueuedEntityOperations();


        void logicUpdate() override;
        Entity& getEntityByName(const std::string& name) override;
        bool doesEntityExist(const std::string& name);
        std::vector<Entity*> getAllEntities() override;
        void addModel(std::string const& name, VertexType vertexType, std::string const& fileName);
        void addModel(std::string const& name, VertexType vertexType, std::string const& fileName, glm::vec3 position, glm::quat quaternion, glm::vec3 scale);

        template<class Vert>
        void addModel(const std::string &name, VertexType vertexType, const std::vector<Vert> &vertices,
                                  const std::vector<uint32_t> &indices) {
            _modelSystem.addModel(name, vertexType, vertices, indices);
        }

        template<class Vert>
        void addModel(const std::string &name, VertexType vertexType, std::vector<Vert> const& vertices,
                      std::vector<uint32_t> const& indices, glm::vec3 position, glm::quat quaternion, glm::vec3 scale) {

            _modelSystem.addModel(name, vertexType, vertices, indices, position, quaternion, scale);
        }
        TModel& getModelByName(std::string const& name);
        void addTexture(std::string const& name, std::string const& fileName);
        BoundTexture& getBoundTextureByName(std::string const& name);
        InputResult getInput();

        std::vector<Entity *>
        getCollidingEntities(Collider *collider, std::unordered_set<std::string> const *targetTags);

        Entity const *getCharacterCollidingEntity(Collider *collider);

        std::unordered_map<VertexType, std::pair<VertexDescriptor, std::set<VertexShader>>> &getAllVertexDescriptors();

        DescriptorSetLayout &getTextureDescriptorSetLayout();

        void windowResizeCallback(GLFWwindow *_window, int width, int height) override;

        std::pair<int, int> getWindowSize() override;

        float getAspectRatio() override;

        void updateGraphicResources(int currentImage);

        void renderFrame(VkCommandBuffer commandBuffer, int currentImage);

        void buildStaticResources();

        void provisionResources(bool initializeDescriptorSets);

        void rebuildResources();

        void cleanupResources();

        void flushPendingResources();
        void destroyPendingResources();

        void destroyResources();


    private:
        explicit GameEngine(BaseProject *bp) : _window(nullptr), _virtualScreenWidthUnits(30.0f),_modelSystem(ModelSystem(bp)),
                                               _textureSystem(TextureSystem(bp)),
                                               _materialSystem(MaterialSystem(bp)), _renderSystem(RenderSystem(bp)),
                                               _inputSystem(), _collisionSystem() {
            _bp = bp;
        };
        BaseProject *_bp;
        static GameEngine *_instance;
        GLFWwindow *_window;
        std::pair<int, int> _windowSize = {800, 600};
        float _virtualScreenWidthUnits;
        std::map<std::string, std::unique_ptr<Entity>> _entities;
        std::map<std::string, std::unique_ptr<Entity>> _enqueuedEntities;
        std::map<std::string, std::pair<DescriptorSet, DescriptorSetInitializationInfo>> _entitiesDescriptorSets;
        std::vector<std::pair<DescriptorSet, int>> _descriptorSetsToClear;
        ModelSystem _modelSystem;
        TextureSystem _textureSystem;
        MaterialSystem _materialSystem;
        RenderSystem _renderSystem;
        InputSystem _inputSystem;
        CollisionSystem _collisionSystem;

        //utils
        std::vector<Component *> getAllComponents();

        void rebuildDescriptorSets();

        void clearDescriptorSets();

        void removeResourcesOfEntity(Entity *entity);

        void removeResourcesOfComponent(Component *component);

        static void
        addEntityToContainer(std::unique_ptr<Entity> entity, std::map<std::string, std::unique_ptr<Entity>> &container);
    };

} // fmwk

#endif //A07_GAMEENGINE_H
