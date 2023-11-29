//
// Created by Anderson on 11/14/2023.
//

#ifndef CASTR_WINDOW_H
#define CASTR_WINDOW_H


#include "GLFW/glfw3.h"

class Window {
public:
    Window(int width, int height, char* name);

    static void initWindowSettings();

    void makeCurrentContext();

    bool shouldClose();

    void swapBuffers();

    GLFWwindow* window;
};


#endif //CASTR_WINDOW_H
