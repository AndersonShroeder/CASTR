//
// Created by Anderson on 11/14/2023.
//

#include "Window.h"

namespace GameState {
    Window::Window(uint16_t width, uint16_t height, uint16_t colorChannels, char *name) : width(width), height(height),
                                                                                          name(name) {
        this->window = glfwCreateWindow(width, height, name, nullptr, nullptr);
    }

    void Window::initWindowSettings() {
        // Set GLFW window hints for OpenGL version and profile
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }

    void Window::makeCurrentContext() const {
        glfwMakeContextCurrent(this->window);
    }

    bool Window::shouldClose() const {
        return glfwWindowShouldClose(this->window);
    }

    void Window::swapBuffers() const {
        glfwSwapBuffers(this->window);
    }

    std::pair<int, int> Window::getWindowDims() const {
        return {this->width, this->height};
    }

    GLFWwindow *Window::getWindow() const {
        return this->window;
    }

}


