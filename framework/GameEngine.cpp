//
// Created by drgio on 02/05/2023.
//

#include "GameEngine.h"
#include "components/materials/MaterialComponent.h"
#include "components/mesh/MeshComponent.h"
#include "components/texture/TextureComponent.h"
#include "components/collision/Collider.h"
#include "components/sprite/Sprite.h"

namespace fmwk {
    GameEngine *GameEngine::_instance = nullptr;

    void GameEngine::createInstance(BaseProject *bp) {
        if (_instance != nullptr) {
            throw std::runtime_error("Cannot initialize game engine twice");
        }
        _instance = new GameEngine(bp);
    }

    GameEngine *GameEngine::getInstance() {
        if (_instance == nullptr) {
            throw std::runtime_error("Cannot obtain GameEngine instance if GameEngine has not been created");
        }
        return _instance;
    }

    std::pair<int, int> GameEngine::getWindowSize() {
        return _windowSize;
    }

    float GameEngine::getAspectRatio() {
        return (float) _windowSize.first / (float) _windowSize.second;
    }

    void GameEngine::windowResizeCallback(GLFWwindow *window, int width, int height) {
        _windowSize = std::pair{width, height};
    }

    Entity &GameEngine::getEntityByName(const std::string &name) {
        auto itr = _entities.find(name);
        if (itr == _entities.end())
            throw std::runtime_error("Could not find entity with name '" + name + "'");
        return *(_entities.find(name)->second);
    }

    std::vector<Entity *> GameEngine::getAllEntities() {
        std::vector<Entity *> entities;
        std::transform(_entities.cbegin(),
                       _entities.cend(),
                       std::back_inserter(entities),
                       [](auto &entry) {
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
        if (elem != _entities.end()) {
            removeResourcesOfEntity(elem->second.get());
            _entities.erase(name);
        }
    }

    void GameEngine::logicUpdate() {
        _inputSystem.captureInputs();

        //std::cout << "Frame rate:" << 1 / getInput().deltaTime << " Entities: " << _entities.size() << std::endl;


        std::vector<Component *> components = getAllComponents();
        for (Component *component: components) {
            component->update();
        }
        flushEnqueuedEntityOperations();
        components = getAllComponents();
        for (Component *component: components) {
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

    std::unordered_map<VertexType, std::pair<VertexDescriptor, std::set<VertexShader>>> &
    GameEngine::getAllVertexDescriptors() {
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

    void GameEngine::bootRenderSystem() {
        _renderSystem.bootSystem(_textureSystem.getTextureDescriptorSetLayout(), _modelSystem.getAllVertexDescriptors(),
                                 _materialSystem.getAllEffects());
    }

    void GameEngine::provisionResources(bool initializeDescriptorSets) {
        auto components = getAllComponents();
        for (Component *component: components) {
            if (!component->isProvisioned()) {
                if (auto *materialComponent = dynamic_cast<MaterialComponent *>(component)) {
                    DescriptorSet descriptorSet;
                    DescriptorSetLayout &dsl = _materialSystem.getEffectByType(
                            materialComponent->getEffectType()).layout;
                    auto *meshComponent = dynamic_cast<MeshComponent *>(&materialComponent->getParent()->getComponentByName(
                            "Mesh"));
                    if (!meshComponent) {
                        throw std::runtime_error("Cannot cast to a Mesh component while provisioning resources");
                    }
                    VertexType vertexType = meshComponent->getModel().getType();
                    Pipeline &pipeline = _renderSystem.getPipeline(vertexType, materialComponent->getEffectType());
                    auto [insertedElement, ok] = _entitiesDescriptorSets.insert(
                            {component->getParent()->getName() + "-" + component->getName(),
                             {descriptorSet, {&dsl, materialComponent->getDescriptorSetClaim()}}});
                    if (initializeDescriptorSets)
                        insertedElement->second.first.init(_bp, &dsl, materialComponent->getDescriptorSetClaim());
                    materialComponent->provision(&insertedElement->second.first, &pipeline);
                } else if (auto *transform = dynamic_cast<Transform *>(component)) {
                    DescriptorSet descriptorSet;
                    DescriptorSetLayout &dsl = _renderSystem.getModelDescriptorSetLayout();
                    DescriptorSetInitializationInfo initializationInfo = {&dsl, {{0, UNIFORM,
                                                                                  sizeof(EntityTransformUniformBlock)}}};
                    auto [insertedElement, ok] = _entitiesDescriptorSets.insert(
                            {component->getParent()->getName() + "-" + component->getName(),
                             {descriptorSet, initializationInfo}});
                    if (initializeDescriptorSets)
                        insertedElement->second.first.init(_bp, &dsl,
                                                           {{0, UNIFORM, sizeof(EntityTransformUniformBlock)}});
                    transform->provision(&insertedElement->second.first);
                } else if (auto *lightComponent = dynamic_cast<LightComponent *>(component)) {
                    lightComponent->provision(&_renderSystem.getGlobalDescriptorSet());
                } else if (auto *colliderComponent = dynamic_cast<Collider *>(component)) {
                    _collisionSystem.addCollider(colliderComponent);
                    colliderComponent->provision();
                } else if (auto *spriteComponent = dynamic_cast<Sprite *>(component)) {
                    DescriptorSet descriptorSet;
                    DescriptorSetLayout &dsl = _renderSystem.getModelOverlayDescriptorSetLayout();
                    DescriptorSetInitializationInfo initializationInfo = {&dsl, {{0, UNIFORM,
                                                                                  sizeof(SpriteUniformBlock)}}};
                    auto [insertedElement, ok] = _entitiesDescriptorSets.insert(
                            {component->getParent()->getName() + "-" + component->getName(),
                             {descriptorSet, initializationInfo}});
                    if (initializeDescriptorSets)
                        insertedElement->second.first.init(_bp, &dsl,
                                                           {{0, UNIFORM, sizeof(SpriteUniformBlock)}});
                    spriteComponent->provision(&insertedElement->second.first);
                } else {
                    throw std::runtime_error("Provision of component '" + component->getName() + "' not implemented");
                }
            }
        }
    }

    void GameEngine::updateGraphicResources(int currentImage) {

        auto allComponents = getAllComponents();
        std::vector<LightComponent *> lights;
        for (Component *component: allComponents) {
            if (auto *lightComponent = dynamic_cast<LightComponent *>(component))
                lights.push_back(lightComponent);
        }
        auto *cameraComponent = dynamic_cast<Camera *>(&getEntityByName("Camera").getComponentByName("Camera"));
        _renderSystem.updateGlobalDescriptor(cameraComponent, lights, getAspectRatio(), _virtualScreenWidthUnits, currentImage);
        for (Component *component: getAllComponents()) {
            if (auto *materialComponent = dynamic_cast<MaterialComponent *>(component)) {
                materialComponent->updateDescriptorSet(currentImage);
            } else if (auto *transformComponent = dynamic_cast<Transform *>(component)) {
                transformComponent->updateDescriptorSet(currentImage);
            } else if(auto *spriteComponent = dynamic_cast<Sprite *>(component)){
                spriteComponent->updateDescriptorSet(currentImage, _virtualScreenWidthUnits, getAspectRatio());
            }
        }
    }

    void GameEngine::renderFrame(VkCommandBuffer commandBuffer, int currentImage) {
        std::vector<Entity *> entities = getAllEntities();
        Pipeline *oldPipeline = nullptr;
        BaseModel *oldModel = nullptr;
        DescriptorSet *oldGlobalDescriptorSet = nullptr;
        BoundTexture *oldTexture = nullptr;


        std::vector<Entity*> nonTransparentEntities{};
        std::vector<Entity*> transparentEntities{};

        for(auto entity : entities){
            if(entity->hasComponent("Material") && _materialSystem.getEffectByType(reinterpret_cast<MaterialComponent&>(entity->getComponentByName("Material")).getEffectType()).isTransparent)
                transparentEntities.push_back(entity);
            else
                nonTransparentEntities.push_back(entity);
        }

        std::sort(transparentEntities.begin(), transparentEntities.end(), [](Entity const* a, Entity const* b){
            return a->getPreferredRenderOrder() >= 0 && a->getPreferredRenderOrder() < b->getPreferredRenderOrder();
        });

        entities.clear();
        for(auto entity : nonTransparentEntities)
            entities.push_back(entity);
        for(auto entity : transparentEntities)
            entities.push_back(entity);

        for (Entity *entity: entities) {
            if (entity->hasComponent("Mesh") && entity->isVisible()) {
                Transform *transform = nullptr;
                Sprite *sprite = nullptr;
                DescriptorSet *globalDescriptorSet = nullptr;
                if(entity->hasComponent("Transform")) {
                    transform = &entity->getTransform();
                    if(globalDescriptorSet != &_renderSystem.getGlobalDescriptorSet()) {
                        globalDescriptorSet = &_renderSystem.getGlobalDescriptorSet();
                        oldTexture = nullptr;
                    }
                }
                else {
                    sprite = reinterpret_cast<Sprite *>(&entity->getComponentByName("Sprite"));
                    if(globalDescriptorSet != &_renderSystem.getGlobalOverlayDescriptorSet()) {
                        globalDescriptorSet = &_renderSystem.getGlobalOverlayDescriptorSet();
                        oldTexture = nullptr;
                    }
                }

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

                if (globalDescriptorSet != oldGlobalDescriptorSet) {
                    globalDescriptorSet->bind(commandBuffer, *oldPipeline, 0, currentImage);
                    oldGlobalDescriptorSet = globalDescriptorSet;
                }

                if (&textureComponent.getBoundTexture() != oldTexture) {
                    textureComponent.getBoundTexture().getDescriptorSet().bind(commandBuffer, *oldPipeline, 1,
                                                                               currentImage);
                    oldTexture = &textureComponent.getBoundTexture();
                }
                materialComponent.getDescriptorSet().bind(commandBuffer, *oldPipeline, 2, currentImage);
                if(transform != nullptr)
                    transform->getDescriptorSet().bind(commandBuffer, *oldPipeline, 3, currentImage);
                else
                    sprite->getDescriptorSet().bind(commandBuffer, *oldPipeline, 3, currentImage);

                vkCmdDrawIndexed(commandBuffer, oldModel->getVertexCount(), 1, 0, 0, 0);
            }
        }


    }

    std::vector<Component *> GameEngine::getAllComponents() {
        std::vector<Component *> components;
        for (Entity *entity: getAllEntities()) {
            for (Component *component: entity->getAllComponents())
                components.push_back(component);
        }
        return components;
    }

    void GameEngine::rebuildDescriptorSets() {
        for (auto &[key, elem]: _entitiesDescriptorSets) {
            elem.first.init(_bp, elem.second.descriptorSetLayout, elem.second.descriptorSetClaim);
        }
    }

    void GameEngine::clearDescriptorSets() {
        for (auto &[key, elem]: _entitiesDescriptorSets) {
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
        destroyPendingResources();
        _textureSystem.destroyTextures();
        _modelSystem.destroyModels();
        _renderSystem.destroyDescriptorSetLayouts();
        _materialSystem.destroyDescriptorSetLayouts();
        _renderSystem.destroyPipelines();
    }

    void GameEngine::removeResourcesOfComponent(Component *component) {
        auto key = component->getParent()->getName() + "-" + component->getName();
        auto elem = _entitiesDescriptorSets.find(key);
        if (elem != _entitiesDescriptorSets.end()) {
            _descriptorSetsToClear.emplace_back(elem->second.first, 3);
            _entitiesDescriptorSets.erase(key);
        }

        _collisionSystem.removeCollider(component->getParent()->getName(), component->getName());
    }

    void GameEngine::removeResourcesOfEntity(Entity *entity) {
        for (Component *component: entity->getAllComponents()) {
            removeResourcesOfComponent(component);
        }
    }

    void GameEngine::addEntityToContainer(std::unique_ptr<Entity> entity,
                                          std::map<std::string, std::unique_ptr<Entity>> &container) {
        if (container.find(entity->getName()) != container.end())
            throw std::runtime_error("Could not add entity with name '" + entity->getName() +
                                     "' because there was another entity with the same name");
        container.insert({entity->getName(), std::move(entity)});
    }

    void GameEngine::enqueueEntityRemoval(const std::string &name) {
        //TODO: decide if making the application crash if the entity is not found or do nothing (in this case it crashes)
        Entity &entity = getEntityByName(name);
        entity.markForRemoval();
    }

    void GameEngine::flushEnqueuedEntityOperations() {
        auto entities = getAllEntities();

        //removing entities and the components marked for removal
        //TODO: provision the components with a lambda that will allow them to free themselves
        for (auto entity: entities) {
            if (entity->isMarkedForRemoval())
                removeEntity(entity->getName());
            else {
                for (Component *component: entity->getAllComponents()) {
                    if (component->isMarkedForRemoval())
                        removeResourcesOfComponent(component);
                }
                entity->flushEnqueuedComponents();
            }
        }

        for (auto &[name, entity]: _enqueuedEntities) {
            addEntity(std::move(entity));
        }
        _enqueuedEntities.clear();
    }

    InputResult GameEngine::getInput() {
        return _inputSystem.handleInputs();
    }

    void GameEngine::bootInputSystem() {
        _inputSystem.bootSystem(_window);
    }

    std::vector<Entity *>
    GameEngine::getCollidingEntities(Collider *collider, std::unordered_set<std::string> const *targetTags) {
        return _collisionSystem.getCollidingEntities(collider, targetTags);
    }

    Entity const *GameEngine::getCharacterCollidingEntity(Collider *collider) {
        return _collisionSystem.getCharacterCollidingEntity(collider);
    }

    void GameEngine::flushPendingResources() {
        auto it = _descriptorSetsToClear.begin();
        while(it != _descriptorSetsToClear.end()){
            it->second--;
            if(it->second <= 0){
                it -> first.cleanup();
                it = _descriptorSetsToClear.erase(it);
            }else it++;
        }
    }

    void GameEngine::addModel(const std::string &name, VertexType vertexType, const std::string &fileName,
                              glm::vec3 position, glm::quat quaternion, glm::vec3 scale) {
        _modelSystem.addModel(name, vertexType, fileName, position, quaternion, scale);
    }

    bool GameEngine::doesEntityExist(const std::string &name) {
        return _entities.find(name) != _entities.end();
    }

    void GameEngine::enqueueRemovalOfAllEntities() {
        for(auto& [name, entity] : _entities)
            enqueueEntityRemoval(name);
    }

    void GameEngine::destroyPendingResources() {
        for(auto& [descriptorSet, val] : _descriptorSetsToClear)
            descriptorSet.cleanup();
        _descriptorSetsToClear.clear();
    }


} // fmwk