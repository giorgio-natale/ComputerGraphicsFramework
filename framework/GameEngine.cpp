//
// Created by drgio on 02/05/2023.
//

#include "GameEngine.h"
#include "MaterialComponent.h"
#include "MeshComponent.h"
#include "TextureComponent.h"

namespace fmwk {
    GameEngine* GameEngine::_instance= nullptr;

    void GameEngine::createInstance(BaseProject *bp){
        if(_instance != nullptr){
            throw std::runtime_error("Cannot initialize game engine twice");
        }
        _instance = new GameEngine(bp);
    }

    GameEngine *GameEngine::getInstance()
    {
        if(_instance==nullptr){
            throw std::runtime_error("Cannot obtain GameEngine instance if GameEngine has not been created");
        }
        return _instance;
    }

    void GameEngine::captureInputs() {
        static auto startTime = std::chrono::high_resolution_clock::now();
        static float lastTime = 0.0f;

        _r = {0,0,0};
        _m = {0,0,0};
        _isFire = false;

        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>
                (currentTime - startTime).count();
        _deltaTime = time - lastTime;
        lastTime = time;

        static double old_xpos = 0, old_ypos = 0;
        double xpos, ypos;
        glfwGetCursorPos(_window, &xpos, &ypos);
        double m_dx = xpos - old_xpos;
        double m_dy = ypos - old_ypos;
        old_xpos = xpos; old_ypos = ypos;

        const float MOUSE_RES = 10.0f;
        glfwSetInputMode(_window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
        if(glfwGetMouseButton(_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            _r.y = -m_dx / MOUSE_RES;
            _r.x = -m_dy / MOUSE_RES;
        }



        if(glfwGetKey(_window, GLFW_KEY_LEFT)) {
            _r.y = 1.0f;
        }
        if(glfwGetKey(_window, GLFW_KEY_RIGHT)) {
            _r.y = -1.0f;
        }
        if(glfwGetKey(_window, GLFW_KEY_UP)) {
            _r.x = 1.0f;
        }
        if(glfwGetKey(_window, GLFW_KEY_DOWN)) {
            _r.x = -1.0f;
        }
        if(glfwGetKey(_window, GLFW_KEY_Q)) {
            _r.z = 1.0f;
        }
        if(glfwGetKey(_window, GLFW_KEY_E)) {
            _r.z = -1.0f;
        }

        if(glfwGetKey(_window, GLFW_KEY_A)) {
            _m.x = -1.0f;
        }
        if(glfwGetKey(_window, GLFW_KEY_D)) {
            _m.x = 1.0f;
        }
        if(glfwGetKey(_window, GLFW_KEY_S)) {
            _m.z = 1.0f;
        }
        if(glfwGetKey(_window, GLFW_KEY_W)) {
            _m.z = -1.0f;
        }
        if(glfwGetKey(_window, GLFW_KEY_R)) {
            _m.y = 1.0f;
        }
        if(glfwGetKey(_window, GLFW_KEY_F)) {
            _m.y = -1.0f;
        }

        _isFire = glfwGetKey(_window, GLFW_KEY_SPACE);
    }

    void GameEngine::handleInputs(float &deltaT, glm::vec3 &m, glm::vec3 &r, bool &fire) {
        deltaT = _deltaTime;
        m = _m;
        r = _r;
        fire = _isFire;
    }

    std::pair<int, int> GameEngine::getWindowSize() {
        return _windowSize;
    }

    float GameEngine::getAspectRatio() {
        return (float)_windowSize.first / (float)_windowSize.second;
    }

    void GameEngine::windowResizeCallback(GLFWwindow *window, int width, int height) {
        _windowSize = std::pair{width, height};
    }

    Entity &GameEngine::getEntityByName(const std::string &name){
        auto itr = _entities.find(name);
        if (itr == _entities.end())
            throw std::runtime_error("Could not find entity with name '" + name+"'");
        return *(_entities.find(name)->second);
    }

    std::vector<Entity *> GameEngine::getAllEntities() {
        std::vector<Entity*> entities;
        std::transform(_entities.cbegin(),
                       _entities.cend(),
                       std::back_inserter(entities),
                       [](auto & entry){
                           return entry.second.get();
                       });
        return entities;
    }

    void GameEngine::addEntity(std::unique_ptr<Entity> entity) {
        GameEngine::addEntityToContainer(std::move(entity), _entities);
    }

    void GameEngine::enqueueEntity(std::unique_ptr<Entity> entity) {
        GameEngine::addEntityToContainer(std::move(entity), _enqueuedEntities);
    }

    void GameEngine::removeEntity(const std::string &name) {
        auto elem = _entities.find(name);
        if(elem != _entities.end()){
            removeResourcesOfEntity(elem->second.get());
            _entities.erase(name);
        }
    }

    void GameEngine::logicUpdate() {
        captureInputs();

        std::vector<Component*> components = getAllComponents();
        for(Component* component : components){
            component->update();
        }
        flushEnqueuedEntityOperations();
        components = getAllComponents();
        for(Component* component : components){
            component->postUpdate();
        }
        flushEnqueuedEntityOperations();

    }

    void GameEngine::addModel(const std::string &name, VertexType vertexType, const std::string &fileName) {
        _modelSystem.addModel(name, vertexType, fileName);
    }

    TModel &GameEngine::getModelByName(const std::string &name) {
        return _modelSystem.getModelByName(name);
    }

    std::unordered_map<VertexType, std::pair<VertexDescriptor, std::string>>& GameEngine::getAllVertexDescriptors(){
        return _modelSystem.getAllVertexDescriptors();
    }

    void GameEngine::addTexture(const std::string &name, const std::string &fileName) {
        _textureSystem.addTexture(name, fileName);
    }

    BoundTexture &GameEngine::getBoundTextureByName(const std::string &name) {
        return _textureSystem.getBoundTextureByName(name);
    }

    DescriptorSetLayout &GameEngine::getTextureDescriptorSetLayout() {
        return _textureSystem.getTextureDescriptorSetLayout();
    }

    void GameEngine::bootTextureSystem() {
        _textureSystem.bootSystem();
    }
    void GameEngine::bootMaterialSystem() {
        _materialSystem.bootSystem();
    }
    void GameEngine::bootRenderSystem(){
        _renderSystem.bootSystem(_textureSystem.getTextureDescriptorSetLayout(), _modelSystem.getAllVertexDescriptors(), _materialSystem.getAllEffects());
    }

    void GameEngine::provisionResources(bool initializeDescriptorSets) {
        auto components = getAllComponents();
        for(Component* component : components){
            if(!component->isProvisioned()){
                if(auto* materialComponent = dynamic_cast<MaterialComponent*>(component)){
                    DescriptorSet descriptorSet;
                    DescriptorSetLayout& dsl = _materialSystem.getEffectByType(materialComponent->getEffectType()).layout;
                    auto* meshComponent = dynamic_cast<MeshComponent*>(&materialComponent->getParent()->getComponentByName("Mesh"));
                    if(!meshComponent){
                        throw std::runtime_error("Cannot cast to a Mesh component while provisioning resources");
                    }
                    VertexType vertexType = meshComponent->getModel().getType();
                    Pipeline& pipeline = _renderSystem.getPipeline(vertexType, materialComponent->getEffectType());
                    auto [insertedElement, ok] = _entitiesDescriptorSets.insert({component->getParent()->getName() + "-" + component->getName(), {descriptorSet, {&dsl, materialComponent->getDescriptorSetClaim()}}});
                    if(initializeDescriptorSets)
                        insertedElement->second.first.init(_bp, &dsl, materialComponent->getDescriptorSetClaim());
                    materialComponent->provision(&insertedElement->second.first, &pipeline);
                }else if(auto* transform = dynamic_cast<Transform*>(component)){
                    DescriptorSet descriptorSet;
                    DescriptorSetLayout& dsl = _renderSystem.getModelDescriptorSetLayout();
                    DescriptorSetInitializationInfo initializationInfo = {&dsl, {{0, UNIFORM, sizeof(EntityTransformUniformBlock)}}};
                    auto [insertedElement, ok] = _entitiesDescriptorSets.insert({component->getParent()->getName() + "-" + component->getName(), {descriptorSet, initializationInfo}});
                    if(initializeDescriptorSets)
                        insertedElement->second.first.init(_bp, &dsl, {{0, UNIFORM, sizeof(EntityTransformUniformBlock)}});
                    transform->provision(&insertedElement->second.first);
                }else{
                    throw std::runtime_error("Provision of component '" + component->getName() + "' not implemented");
                }
            }
        }
    }

    void GameEngine::updateGraphicResources(int currentImage) {

        auto* cameraComponent = dynamic_cast<Camera*>(&getEntityByName("Camera").getComponentByName("Camera"));
        _renderSystem.updateGlobalDescriptor(cameraComponent, currentImage);
        for(Component* component : getAllComponents()){
            if(auto* materialComponent = dynamic_cast<MaterialComponent*>(component)){
                materialComponent->updateDescriptorSet(currentImage);
            }else if(auto* transformComponent = dynamic_cast<Transform*>(component)){
                transformComponent->updateDescriptorSet(currentImage);
            }
        }
    }

    void GameEngine::renderFrame(VkCommandBuffer commandBuffer, int currentImage) {
        std::vector<Entity*> entities = getAllEntities();
        Pipeline* oldPipeline = nullptr;
        BaseModel* oldModel = nullptr;
        DescriptorSet* globalDescriptorSet = &_renderSystem.getGlobalDescriptorSet();
        BoundTexture* oldTexture = nullptr;

        bool needToBindGlobalDescriptor = true;
        for(Entity* entity : entities){
            if(entity->hasComponent("Mesh")) {
                Transform &transform = entity->getTransform();
                MeshComponent &meshComponent = reinterpret_cast<MeshComponent &>(entity->getComponentByName("Mesh"));
                MaterialComponent &materialComponent = reinterpret_cast<MaterialComponent &>(entity->getComponentByName(
                        "Material"));
                TextureComponent &textureComponent = reinterpret_cast<TextureComponent &>(entity->getComponentByName(
                        "Texture"));

                if (&meshComponent.getModel().getTypedModel() != oldModel) {
                    meshComponent.getModel().getTypedModel().bind(commandBuffer);
                    oldModel = &meshComponent.getModel().getTypedModel();
                }

                if (materialComponent.getPipeline() != oldPipeline) {
                    materialComponent.getPipeline()->bind(commandBuffer);
                    oldPipeline = materialComponent.getPipeline();
                }

                if (needToBindGlobalDescriptor) {
                    globalDescriptorSet->bind(commandBuffer, *oldPipeline, 0, currentImage);
                    needToBindGlobalDescriptor = false;
                }
                if (&textureComponent.getBoundTexture() != oldTexture) {
                    textureComponent.getBoundTexture().getDescriptorSet().bind(commandBuffer, *oldPipeline, 1, currentImage);
                    oldTexture = &textureComponent.getBoundTexture();
                }
                materialComponent.getDescriptorSet().bind(commandBuffer, *oldPipeline, 2, currentImage);
                transform.getDescriptorSet().bind(commandBuffer, *oldPipeline, 3, currentImage);

                vkCmdDrawIndexed(commandBuffer, oldModel->getVertexCount(), 1, 0, 0, 0);
            }
        }



    }

    std::vector<Component *> GameEngine::getAllComponents() {
        std::vector<Component*> components;
        for(Entity* entity : getAllEntities()){
            for(Component* component : entity->getAllComponents())
                components.push_back(component);
        }
        return components;
    }

    void GameEngine::rebuildDescriptorSets() {
        for(auto& [key, elem] : _entitiesDescriptorSets){
            elem.first.init(_bp, elem.second.descriptorSetLayout, elem.second.descriptorSetClaim);
        }
    }

    void GameEngine::clearDescriptorSets() {
        for(auto& [key, elem] : _entitiesDescriptorSets){
            elem.first.cleanup();
        }
    }

    void GameEngine::buildStaticResources() {
        _renderSystem.rebuildPipelines();
        _renderSystem.rebuildGlobalDescriptorSet();
        _textureSystem.rebuildTextureDescriptorSets();
    }

    void GameEngine::rebuildResources() {
        buildStaticResources();
        rebuildDescriptorSets();
    }

    void GameEngine::cleanupResources() {
        _renderSystem.resetPipelines();
        _renderSystem.cleanupGlobalDescriptorSet();
        _textureSystem.resetTextureDescriptorSets();
        clearDescriptorSets();
    }

    void GameEngine::destroyResources() {

    }

    void GameEngine::removeResourcesOfEntity(Entity *entity) {
        for(Component* component : entity->getAllComponents()){
            auto key = entity->getName() + "-" + component->getName();
            auto elem = _entitiesDescriptorSets.find(key);
            if(elem != _entitiesDescriptorSets.end()){
                elem->second.first.cleanup();
                _entitiesDescriptorSets.erase(key);
            }
        }
    }

    void GameEngine::addEntityToContainer(std::unique_ptr<Entity> entity,
                                          std::map<std::string, std::unique_ptr<Entity>>& container) {
        if(container.find(entity->getName()) != container.end())
            throw std::runtime_error("Could not add component with name '" + entity->getName() + "' because there was another entity with the same name");
        container.insert({entity->getName(), std::move(entity)});
    }

    void GameEngine::enqueueEntityRemoval(const std::string &name) {
        //TODO: decide if making the application crash if the entity is not found or do nothing (in this case it crashes)
        Entity& entity = getEntityByName(name);
        entity.markForRemoval();
    }

    void GameEngine::flushEnqueuedEntityOperations() {
        auto entities = getAllEntities();
        for(auto& entity : entities){
            if(entity->isMarkedForRemoval())
                removeEntity(entity->getName());
        }

        for(auto& [name, entity] : _enqueuedEntities){
            addEntity(std::move(entity));
        }
        _enqueuedEntities.clear();
    }


} // fmwk