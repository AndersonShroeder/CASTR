//
// Created by Anderson on 11/14/2023.
//

#include "Window.h"

Window::Window(int width, int height, char *name) {
    this->window = glfwCreateWindow(width, height, name, NULL, NULL);
}

void Window::initWindowSettings() {
    // Set GLFW window hints for OpenGL version and profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void Window::makeCurrentContext() {
    glfwMakeContextCurrent(this->window);
}

bool Window::shouldClose() {
    return glfwWindowShouldClose(this->window);
}

void Window::swapBuffers() {
    glfwSwapBuffers(this->window);
}


