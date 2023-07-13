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

namespace fmwk {

    class GameEngine{
    public:
        GameEngine(GameEngine &other) = delete;
        void operator=(const GameEngine &) = delete;
        static GameEngine* getInstance();
        void setWindow(GLFWwindow* window){
            _window = window;
        }

        void addEntity(std::unique_ptr<Entity> entity);
        void logicUpdate();
        Entity& getEntityByName(const std::string& name);
        std::vector<Entity*> getAllEntities();

        void handleInputs(float &deltaT, glm::vec3 &m, glm::vec3 &r, bool &fire);
        void windowResizeCallback(GLFWwindow* _window, int width, int height);
        std::pair<int, int> getWindowSize();
        float getAspectRatio();

    private:
        GameEngine(){
            _window = nullptr;
        };
        static GameEngine* _instance;
        GLFWwindow* _window;
        std::pair<int, int> _windowSize = {800, 600};
        std::map<std::string, std::unique_ptr<Entity>> _entities;

        glm::vec3 _r = {0,0,0}, _m = {0,0,0};
        float _deltaTime = 0;
        bool _isFire = false;
        void captureInputs();
    };

} // fmwk

#endif //A07_GAMEENGINE_H
