#include "imports.h"
#include "Geometry/Geometry.h"
#include "Shaders/shaders.h"
#include "Rendering/Renderer.h"
#include "Rendering/Shader.h"
#include "Window/Window.h"
#include "Vector/Vector.h"
#include "Entities/Entity.h"
#include "Entities/Player.h"

void init(){
    // Initialize GLFW
    glfwInit();

    // Initialize window settings
    Window::initWindowSettings();

    // Set the swap interval (vertical sync) to 1 (enabled)
    glfwSwapInterval(1);
}

Keys InputReader::keys;
int main() {
    init();
    Window window{800, 800, "Window 1"};
    window.makeCurrentContext();
    gladLoadGL();

//    Rendering
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    BasicShapeRenderer renderer;
    Shader shader{vertexShaderSource, fragmentShaderSource};

    PositionInfo info{{20, 12}, {-1, 0}, {0, .66}};
    Player player;
    player.updatePositionInfo(info);

    while (!window.shouldClose()) {
        // Check for user input
        if (glfwGetKey(window.window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            // ESC key is pressed, close the window
            glfwSetWindowShouldClose(window.window, true);
        }

        glfwSetKeyCallback(window.window, InputReader::readKeys);
        window.swapBuffers();
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);

    // Set the OpenGL viewport to match the window dimensions
    glViewport(0, 0, 800, 800);

    return 0;
}
