#include "imports.h"
#include "Geometry/Geometry.h"
#include "Shaders/shaders.h"
#include "Rendering/Renderer.h"
#include "Rendering/Shader.h"
#include "Window/Window.h"

void init(){
    // Initialize GLFW
    glfwInit();

    // Initialize window settings
    Window::initWindowSettings();

    // Set the swap interval (vertical sync) to 1 (enabled)
    glfwSwapInterval(1);
}

int main() {
    init();

    Window window{800, 800, "Window 1"};
    window.makeCurrentContext();

    // Load OpenGL function pointers using GLAD
    gladLoadGL();

    Triangles t = Triangles({0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
                                    0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                                    1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f}, {0, 1, 2}, 10);

    GLuint VAO;
    glGenVertexArrays(1, &VAO);

    BasicShapeRenderer renderer;
    Shader shader{vertexShaderSource, fragmentShaderSource};

    while (!window.shouldClose()) {
        renderer.render(t, shader, VAO);
        window.swapBuffers();
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);

    // Set the OpenGL viewport to match the window dimensions
    glViewport(0, 0, 800, 800);

    return 0;
}
