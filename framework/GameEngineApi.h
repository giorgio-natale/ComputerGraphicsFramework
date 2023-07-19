//
// Created by drgio on 19/07/2023.
//

#ifndef DEMO_GAMEENGINEAPI_H
#define DEMO_GAMEENGINEAPI_H

#include <GLFW/glfw3.h>
#include <memory>
#include "Entity.h"

namespace fmwk{
    class GameEngineApi{
    public:
        virtual void setWindow(GLFWwindow* window) = 0;
        virtual void addEntity(std::unique_ptr<Entity> entity) = 0;
        virtual void logicUpdate() = 0;
        virtual Entity& getEntityByName(const std::string& name) = 0;
        virtual std::vector<Entity*> getAllEntities() = 0;
        virtual void handleInputs(float &deltaT, glm::vec3 &m, glm::vec3 &r, bool &fire) = 0;
        virtual void windowResizeCallback(GLFWwindow* _window, int width, int height) = 0;
        virtual std::pair<int, int> getWindowSize() = 0;
        virtual float getAspectRatio() = 0;
        virtual ~GameEngineApi()= default;
    };
}
#endif //DEMO_GAMEENGINEAPI_H
