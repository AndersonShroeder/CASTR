#include "imports.h"
#include "Geometry/Geometry.h"
#include "Shaders/shaders.h"
#include "Rendering/Renderer.h"
#include "Rendering/Shader.h"
#include "Window/Window.h"
#include "Vector/Vector.h"
#include "Entities/Entity.h"
#include "Entities/Player.h"
#include "Camera/Camera.h"

#define mapWidth 24
#define mapHeight 24
#define screenWidth 1920
#define screenHeight 1080

int worldMap[mapWidth][mapHeight]=
        {
                {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
                {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
                {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
                {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
        };

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
    Window window{screenWidth, screenHeight, "Window 1"};
    window.makeCurrentContext();
    gladLoadGL();

    // Setup Input reading
    glfwSetKeyCallback(window.window, InputReader::readKeys);

    // Setup VAO
    GLuint VAO;
    glGenVertexArrays(1, &VAO);

    // Setup Renderer
    BasicShapeRenderer renderer;
    RayCastRenderer caster;
    Shader shader{vertexShaderSource, fragmentShaderSource};

    // Setup Player
    PositionInfo2D info{{22, 12}, {-1, 0}, {0, .66}};
    Camera cam{};
    Player player;
    player.updatePositionInfo(info);
    cam.subscribe(player);

    while (!window.shouldClose()) {
        glClear(GL_COLOR_BUFFER_BIT);

        Lines l = caster.DDA(screenWidth, screenHeight, cam.getPositionInfo(), worldMap);
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
