//
// Created by drgio on 02/05/2023.
//

#include "GameEngine.h"

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
        if(_entities.find(entity->getName()) != _entities.end())
            throw std::runtime_error("Could not add component with name '" + entity->getName() + "' because there was another entity with the same name");
        _entities.insert({entity->getName(), std::move(entity)});
    }

    void GameEngine::logicUpdate() {
        captureInputs();
        std::for_each(_entities.begin(), _entities.end(),[](auto& entry){
            auto components = entry.second->getAllComponents();
            std::for_each(components.begin(), components.end(),[](auto& component){
                component->update();
            });
        });
        std::for_each(_entities.begin(), _entities.end(),[](auto& entry){
            auto components = entry.second->getAllComponents();
            std::for_each(components.begin(), components.end(),[](auto& component){
                component->postUpdate();
            });
        });
    }

    void GameEngine::addModel(const std::string &name, VertexType vertexType, const std::string &fileName) {
        _modelSystem.addModel(name, vertexType, fileName);
    }

    BaseModel &GameEngine::getModelByName(const std::string &name) {
        return _modelSystem.getModelByName(name);
    }

    std::unordered_map<VertexType, VertexDescriptor> &GameEngine::getAllVertexDescriptors() {
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
} // fmwk