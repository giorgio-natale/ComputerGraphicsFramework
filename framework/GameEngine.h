//
// Created by drgio on 02/05/2023.
//

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

//#include "systems/ModelSystem.h"
#include "../Starter.hpp"
#include "systems/ModelSystem.h"
#include "systems/TextureSystem.h"
#include "systems/MaterialSystem.h"
#include "systems/RenderSystem.h"

namespace fmwk {

    struct DescriptorSetInitializationInfo{
        DescriptorSetLayout* descriptorSetLayout;
        DescriptorSetClaim descriptorSetClaim;
    };

    class GameEngine:GameEngineApi{
    public:
        GameEngine(GameEngine &other) = delete;
        void operator=(const GameEngine &) = delete;
        static GameEngine* getInstance();
        static void createInstance(BaseProject* bp);
        void setWindow(GLFWwindow* window) override{
            _window = window;
        }
        void bootTextureSystem();
        void bootMaterialSystem();
        void bootRenderSystem();

        void addEntity(std::unique_ptr<Entity> entity) override;
        void enqueueEntity(std::unique_ptr<Entity> entity);
        void removeEntity(std::string const& name);
        void enqueueEntityRemoval(std::string const& name);
        void flushEnqueuedEntityOperations();
        void logicUpdate() override;
        Entity& getEntityByName(const std::string& name) override;
        std::vector<Entity*> getAllEntities() override;
        void addModel(std::string const& name, VertexType vertexType, std::string const& fileName);
        TModel& getModelByName(std::string const& name);
        void addTexture(std::string const& name, std::string const& fileName);
        BoundTexture& getBoundTextureByName(std::string const& name);

        //TODO: remove these
        std::unordered_map<VertexType, std::pair<VertexDescriptor, std::string>>& getAllVertexDescriptors();
        DescriptorSetLayout& getTextureDescriptorSetLayout();

        void handleInputs(float &deltaT, glm::vec3 &m, glm::vec3 &r, bool &fire) override;
        void windowResizeCallback(GLFWwindow* _window, int width, int height) override;
        std::pair<int, int> getWindowSize() override;
        float getAspectRatio() override;
        void updateGraphicResources(int currentImage);
        void renderFrame(VkCommandBuffer commandBuffer, int currentImage);

        //TODO: put this in a separate centralized system responsible for resource allocation
        void buildStaticResources();
        void provisionResources(bool initializeDescriptorSets);
        void rebuildResources();
        void cleanupResources();
        void destroyResources();


    private:
        explicit GameEngine(BaseProject* bp):_window(nullptr), _modelSystem(ModelSystem(bp)), _textureSystem(TextureSystem(bp)),
                                             _materialSystem(MaterialSystem(bp)), _renderSystem(RenderSystem(bp)){
            _bp = bp;
        };
        BaseProject* _bp;
        static GameEngine* _instance;
        GLFWwindow* _window;
        std::pair<int, int> _windowSize = {800, 600};
        std::map<std::string, std::unique_ptr<Entity>> _entities;
        std::map<std::string, std::unique_ptr<Entity>> _enqueuedEntities;
        std::map<std::string, std::pair<DescriptorSet, DescriptorSetInitializationInfo>> _entitiesDescriptorSets;
        ModelSystem _modelSystem;
        TextureSystem _textureSystem;
        MaterialSystem _materialSystem;
        RenderSystem _renderSystem;

        //utils
        std::vector<Component*> getAllComponents();
        void rebuildDescriptorSets();
        void clearDescriptorSets();
        void removeResourcesOfEntity(Entity* entity);

        static void addEntityToContainer(std::unique_ptr<Entity> entity, std::map<std::string, std::unique_ptr<Entity>>& container);

        glm::vec3 _r = {0,0,0}, _m = {0,0,0};
        float _deltaTime = 0;
        bool _isFire = false;
        void captureInputs();
    };

} // fmwk

#endif //A07_GAMEENGINE_H
