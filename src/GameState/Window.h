// Created by Anderson on 11/14/2023.

/**
 * @file GameState.h
 * @brief Declaration of the GameState class for managing GLFW windows.
 */

#ifndef CASTR_WINDOW_H
#define CASTR_WINDOW_H

#include <utility>
#include <vector>
#include "GLFW/glfw3.h"
namespace GameState {
/**
 * @brief Class for managing GLFW windows.
 */
    class Window {
    public:
        /**
         * @brief Constructor for creating a window with specified width, height, and name.
         * @param width The width of the window.
         * @param height The height of the window.
         * @param name The name/title of the window.
         */
        Window(uint16_t width, uint16_t height, uint16_t colorChannels, char *name);

        /**
         * @brief Initializes GLFW window settings. Should be called before creating any window.
         */
        static void initWindowSettings();

        /**
         * @brief Makes the current window's OpenGL context active.
         */
        void makeCurrentContext() const;

        /**
         * @brief Checks if the window should close.
         * @return True if the window should close; otherwise, false.
         */
        [[nodiscard]] bool shouldClose() const;

        /**
         * @brief Swaps the front and back buffers of the window.
         */
        void swapBuffers() const;

        /**
         * @brief The GLFW window handle.
         */

        [[nodiscard]] GLFWwindow *getWindow() const;

        [[nodiscard]] std::pair<int, int> getWindowDims() const;

    private:
        uint16_t width;
        uint16_t height;
        char *name;
        GLFWwindow *window;
    };
}
#endif //CASTR_WINDOW_H
