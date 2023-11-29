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
    // Initialization
    init();
    Window window{800, 800, "Window 1"};
    window.makeCurrentContext();
    gladLoadGL();

    // Setup Input reading
    glfwSetKeyCallback(window.window, InputReader::readKeys);

    // Setup VAO
    GLuint VAO;
    glGenVertexArrays(1, &VAO);

    // Setup Renderer
    BasicShapeRenderer renderer;
    Shader shader{vertexShaderSource, fragmentShaderSource};

    // Setup Player
    PositionInfo info{{0, 0}, {-1, 0}, {0, .66}};
    Player player;
    player.updatePositionInfo(info);

    while (!window.shouldClose()) {
        glClear(GL_COLOR_BUFFER_BIT);

        // Input Reading demo
        vDouble2d coord = player.getPositionInfo().pos + player.getPositionInfo().dir;
        vVertex v{static_cast<float>(player.getPositionInfo().pos[0]), static_cast<float>(player.getPositionInfo().pos[1]), 0, 1, 0 ,0,
                  static_cast<float>(coord[0]), static_cast<float>(coord[1]), 0, 1, 0, 0};
        vIndex i{0, 1};

        Lines l{v, i, 10};
        renderer.render(l, shader, VAO);
        player.readInput();

        window.swapBuffers();
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);

    // Set the OpenGL viewport to match the window dimensions
    glViewport(0, 0, 800, 800);

    return 0;
}
