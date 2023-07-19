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

namespace fmwk {

    class GameEngine:GameEngineApi{
    public:
        GameEngine(GameEngine &other) = delete;
        void operator=(const GameEngine &) = delete;
        static GameEngine* getInstance();
        static void createInstance(BaseProject* bp);
        void setWindow(GLFWwindow* window) override{
            _window = window;
        }

        void addEntity(std::unique_ptr<Entity> entity) override;
        void logicUpdate() override;
        Entity& getEntityByName(const std::string& name) override;
        std::vector<Entity*> getAllEntities() override;
        void addModel(std::string const& name, VertexType vertexType, std::string const& fileName);
        BaseModel& getModelByName(std::string const& name);
        //TODO: remove this
        std::unordered_map<VertexType, VertexDescriptor>& getAllVertexDescriptors();

        void handleInputs(float &deltaT, glm::vec3 &m, glm::vec3 &r, bool &fire) override;
        void windowResizeCallback(GLFWwindow* _window, int width, int height) override;
        std::pair<int, int> getWindowSize() override;
        float getAspectRatio() override;

    private:
        explicit GameEngine(BaseProject* bp):_window(nullptr), _modelSystem(ModelSystem(bp)){
            _bp = bp;
        };
        BaseProject* _bp;
        static GameEngine* _instance;
        GLFWwindow* _window;
        std::pair<int, int> _windowSize = {800, 600};
        std::map<std::string, std::unique_ptr<Entity>> _entities;
        ModelSystem _modelSystem;

        glm::vec3 _r = {0,0,0}, _m = {0,0,0};
        float _deltaTime = 0;
        bool _isFire = false;
        void captureInputs();
    };

} // fmwk

#endif //A07_GAMEENGINE_H
