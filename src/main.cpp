#include <iostream>
#include "imports.h"
#include "Rendering/Renderer.h"
#include "Camera/Camera.h"
#include "Entities/Player.h"
#include "Window/Window.h"
#include "Shaders/shaders.h"

#define mapWidth 24
#define mapHeight 24
#define screenWidth 800
#define screenHeight 800
#define texWidth 64
#define texHeight 64

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
    glBindVertexArray(VAO);

    // Generate an array of RGB pixel data
    vVertex vertices = {
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 0.0f, 1.0f, 1.0f
    };
    GLubyte* pixels = new GLubyte[800 * 800 * 3];
    TextureQuad quad{vertices, 800, 800, pixels};

    // Setup Renderer
    RayCastRenderer caster(quad);
    Shader shader{vertexShaderSource, fragmentShaderSource};
    shader.useShader();

    // Setup Player
    PositionInfo2D info{{22, 12}, {-1, 0}, {0, .66}};
    Camera cam{};
    Player player;
    player.updatePositionInfo(info);
    cam.subscribe(player);

    // Main loop
    while (!window.shouldClose()) {
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT);

        caster.render(shader, VAO, cam.getPositionInfo(),worldMap);
        player.readInput();

        // Swap buffers
        window.swapBuffers();

        // Poll for and process events
        glfwPollEvents();
    }

    // Clean up
//    glDeleteTextures(1, &textureID);
    glDeleteVertexArrays(1, &VAO);
//    glDeleteBuffers(1, &VBO);
//    glDeleteProgram(shaderProgram);

    // Terminate GLFW
    glfwTerminate();

    delete[] pixels;

    return 0;
}
