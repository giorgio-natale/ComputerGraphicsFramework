//
// Created by drgio on 24/07/2023.
//

#include <chrono>
#include "InputSystem.h"
#include "../../Starter.hpp"

namespace fmwk {
    void InputSystem::captureInputs() {
        static auto startTime = std::chrono::high_resolution_clock::now();
        static float lastTime = 0.0f;

        _inputResult.r = {0,0,0};
        _inputResult.m = {0,0,0};
        _inputResult.spacePressed = false;

        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>
                (currentTime - startTime).count();
        _inputResult.deltaTime = time - lastTime;
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
            _inputResult.r.y = -m_dx / MOUSE_RES;
            _inputResult.r.x = -m_dy / MOUSE_RES;
        }



        if(glfwGetKey(_window, GLFW_KEY_LEFT)) {
            _inputResult.r.y = 1.0f;
        }
        if(glfwGetKey(_window, GLFW_KEY_RIGHT)) {
            _inputResult.r.y = -1.0f;
        }
        if(glfwGetKey(_window, GLFW_KEY_UP)) {
            _inputResult.r.x = 1.0f;
        }
        if(glfwGetKey(_window, GLFW_KEY_DOWN)) {
            _inputResult.r.x = -1.0f;
        }
        if(glfwGetKey(_window, GLFW_KEY_Q)) {
            _inputResult.r.z = 1.0f;
        }
        if(glfwGetKey(_window, GLFW_KEY_E)) {
            _inputResult.r.z = -1.0f;
        }

        if(glfwGetKey(_window, GLFW_KEY_A)) {
            _inputResult.m.x = -1.0f;
        }
        if(glfwGetKey(_window, GLFW_KEY_D)) {
            _inputResult.m.x = 1.0f;
        }
        if(glfwGetKey(_window, GLFW_KEY_S)) {
            _inputResult.m.z = 1.0f;
        }
        if(glfwGetKey(_window, GLFW_KEY_W)) {
            _inputResult.m.z = -1.0f;
        }
        if(glfwGetKey(_window, GLFW_KEY_R)) {
            _inputResult.m.y = 1.0f;
        }
        if(glfwGetKey(_window, GLFW_KEY_F)) {
            _inputResult.m.y = -1.0f;
        }

        _inputResult.spacePressed = glfwGetKey(_window, GLFW_KEY_SPACE);
    }

    InputResult InputSystem::handleInputs() {
        return _inputResult;
    }

    void InputSystem::bootSystem(GLFWwindow *window) {
        _window = window;
    }
} // fmwk