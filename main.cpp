#include "imports.h"
#include "Geometry/Geometry.h"
#include "shaders.h"
#include "Rendering/Renderer.h"
#include "Rendering/Shader.h"

int main() {
    // Initialize GLFW
    glfwInit();

    // Set GLFW window hints for OpenGL version and profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a GLFW window with the specified parameters
    auto window = glfwCreateWindow(800, 800, "name", NULL, NULL);


    // Make the created window the current OpenGL context
    glfwMakeContextCurrent(window);

    // Load OpenGL function pointers using GLAD
    gladLoadGL();

    // Set the swap interval (vertical sync) to 1 (enabled)
    glfwSwapInterval(1);

    Triangles t = Triangles({0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
                                    0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                                    1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f}, {0, 1, 2}, 10);
    BasicShapeRenderer renderer;

    GLuint VAO;
    glGenVertexArrays(1, &VAO);

    Shader shader{vertexShaderSource, fragmentShaderSource};

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        renderer.render(t, shader, VAO);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    // Set the OpenGL viewport to match the window dimensions
    glViewport(0, 0, 800, 800);

    std::cout << "Hello, World!" << std::endl;
    return 0;
}
