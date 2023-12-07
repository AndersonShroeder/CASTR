#include <iostream>
#include "imports.h"
#include "Rendering/Renderer.h"
#include "Entities/Camera.h"
#include "Entities/Player.h"
#include "GameState//Window.h"
#include "shaders.h"
#include "GameState/GameLogic.h"

#define mapWidth 24
#define mapHeight 24
#define screenWidth 1920
#define screenHeight 1080
#define resolutionWidth 1920
#define resolutionHeight 1080
#define texWidth 64
#define texHeight 64


std::vector<std::vector<int>> worldMap=
        {
                {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,7,7,7,7,7,7,7,7},
                {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7},
                {4,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
                {4,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
                {4,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7},
                {4,0,4,0,0,0,0,5,5,5,5,5,5,5,5,5,7,7,0,7,7,7,7,7},
                {4,0,5,0,0,0,0,5,0,5,0,5,0,5,0,5,7,0,0,0,7,7,7,1},
                {4,0,6,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,8},
                {4,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,7,7,1},
                {4,0,8,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,8},
                {4,0,0,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,7,7,7,1},
                {4,0,0,0,0,0,0,5,5,5,5,0,5,5,5,5,7,7,7,7,7,7,7,1},
                {6,6,6,6,6,6,6,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6},
                {8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
                {6,6,6,6,6,6,0,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6},
                {4,4,4,4,4,4,0,4,4,4,6,0,6,2,2,2,2,2,2,2,3,3,3,3},
                {4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2},
                {4,0,0,0,0,0,0,0,0,0,0,0,6,2,0,0,5,0,0,2,0,0,0,2},
                {4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2},
                {4,0,6,0,6,0,0,0,0,4,6,0,0,0,0,0,5,0,0,0,0,0,0,2},
                {4,0,0,5,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2},
                {4,0,6,0,6,0,0,0,0,4,6,0,6,2,0,0,5,0,0,2,0,0,0,2},
                {4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2},
                {4,4,4,4,4,4,4,4,4,4,1,1,1,2,2,2,2,2,2,3,3,3,3,3}
        };

void init(){
    // Initialize GLFW
    glfwInit();

    // Initialize window settings
    GameState::Window::initWindowSettings();

    // Set the swap interval (vertical sync) to 1 (enabled)
    glfwSwapInterval(1);
}


GameState::Keys GameState::InputReader::keys;
int main() {
    // Initialization
    init();
    GameState::Window window{screenWidth, screenHeight, 3, "GameState 1"};
    window.makeCurrentContext();
    gladLoadGL();

    // Setup Input reading
    glfwSetKeyCallback(window.getWindow(), GameState::InputReader::readKeys);

    // Setup VAO
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    Rendering::TextureQuad quad{resolutionWidth, resolutionHeight, 3};

    // Setup Renderer
    Rendering::TextureRenderer renderer;
    Rendering::Shader shader{vertexShaderSource, fragmentShaderSource};
    shader.useShader();

    // Setup Player
    Entities::PositionInfo2D info{{22, 12}, {-1, 0}, {0, .66}};
    Entities::Player player;
    player.updatePositionInfo(info);

    // Establish Rendering POV -> POV of player
    Entities::Camera cam{};
    cam.subscribe(player);

    // Setup Raycasting logic
    GameState::MapData map{worldMap};
    GameState::RayCasterLogic rayCaster(quad, texWidth, texHeight, map);

    // Main loop
    while (!window.shouldClose()) {
        // Render quad after raycasting
        rayCaster.DDA(player.getPositionInfo());
        renderer.render(quad, shader, VAO);

        // Clear Quad
        quad.clearTextureData();

        player.readInput();
        window.swapBuffers();
        glfwPollEvents();
    }


    glDeleteVertexArrays(1, &VAO);
    // Clean up
//    glDeleteTextures(1, &textureID);
//    glDeleteBuffers(1, &VBO);
//    glDeleteProgram(shaderProgram);

    // Terminate GLFW
    glfwTerminate();

    return 0;
}
