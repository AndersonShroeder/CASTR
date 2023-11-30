// Created by Anderson on 11/28/2023.

/**
 * @file InputReader.h
 * @brief Declaration of the InputReader class and associated Keys structure for handling input.
 */

#include "../imports.h"

#ifndef CASTR_INPUTREADER_H
#define CASTR_INPUTREADER_H

/**
 * @brief Structure to store the state of various keys.
 */
struct Keys
{
    bool e_key = false; /**< State of the 'E' key. */
    bool q_key = false; /**< State of the 'Q' key. */
    bool w_key = false; /**< State of the 'W' key. */
    bool a_key = false; /**< State of the 'A' key. */
    bool s_key = false; /**< State of the 'S' key. */
    bool d_key = false; /**< State of the 'D' key. */
    bool shift_key = false; /**< State of the left shift key. */
};

/**
 * @brief Class for handling input events and updating key states.
 */
class InputReader {
public:
    /**
     * @brief Static instance of the Keys structure to store key states.
     */
    static Keys keys;

    /**
     * @brief Function to read GLFW key events and update key states accordingly.
     * @param window The GLFW window.
     * @param key The pressed/released key.
     * @param scancode The system-specific scancode of the key.
     * @param action The key action (GLFW_PRESS, GLFW_RELEASE, GLFW_REPEAT).
     * @param mods Bit field describing which modifier keys were held down.
     */
    static void readKeys(GLFWwindow *window, int key, int scancode, int action, int mods) {
        bool truthVal = action != GLFW_RELEASE;
        switch(key) {
            case GLFW_KEY_E: keys.e_key = truthVal; break;
            case GLFW_KEY_Q: keys.q_key = truthVal; break;
            case GLFW_KEY_W: keys.w_key = truthVal; break;
            case GLFW_KEY_S: keys.s_key = truthVal; break;
            case GLFW_KEY_A: keys.a_key = truthVal; break;
            case GLFW_KEY_D: keys.d_key = truthVal; break;
            case GLFW_KEY_LEFT_SHIFT: keys.shift_key = truthVal; break;
            default: break;
        }
    }

    /**
     * @brief Pure virtual function for reading input. Must be implemented by derived classes.
     */
    virtual void readInput() = 0;
};

#endif //CASTR_INPUTREADER_H
