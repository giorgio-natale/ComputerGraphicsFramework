//
// Created by drgio on 24/07/2023.
//

#ifndef DEMO_INPUTSYSTEM_H
#define DEMO_INPUTSYSTEM_H

#include <glm/vec3.hpp>
#include <GLFW/glfw3.h>

namespace fmwk {

    struct InputResult{
        glm::vec3 r;
        glm::vec3 m;
        float deltaTime;
        bool spacePressed;
    };

    class InputSystem {
    public:
        void captureInputs();
        InputResult handleInputs();
        void bootSystem(GLFWwindow* window);
    private:
        InputResult _inputResult;
        GLFWwindow* _window;
    };

} // fmwk

#endif //DEMO_INPUTSYSTEM_H
