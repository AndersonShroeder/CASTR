//
// Created by Anderson on 11/28/2023.
//

#include "../imports.h"

#ifndef CASTR_INPUTREADER_H
#define CASTR_INPUTREADER_H

struct Keys
 {
     bool e_key = false;
     bool q_key = false;
     bool w_key = false;
     bool a_key = false;
     bool s_key = false;
     bool d_key = false;
     bool shift_key = false;
 };

class InputReader {
public:
    static Keys keys;
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

    virtual void readInput() = 0;
};

#endif //CASTR_INPUTREADER_H
