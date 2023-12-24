#include "imports.h"
#include "Rendering/Renderer.h"
#include "Entities/Player.h"
#include "GameState//Window.h"
#include "shaders.h"
#include "GameState/GameLogic.h"

#define screenWidth 800
#define screenHeight 800
#define resolutionWidth 800
#define resolutionHeight 800
#define texWidth 64
#define texHeight 64

void init(){
    // Initialize GLFW
    glfwInit();

    // Initialize window settings
    GameState::Window::initWindowSettings();
}

namespace RayCaster {
    void run(GameState::Window window, Rendering::TextureQuad quad, Rendering::TextureRenderer renderer, Rendering::Shader shader, GLuint VAO) {
        // Setup Player
        Entities::Player2D player;

        // Setup Raycasting logic
        GameState::RayCasterLogic rayCaster(quad, texWidth, texHeight);
        rayCaster.changeMap("C:\\Users\\Anderson\\CLionProjects\\CASTR\\src\\RayCaster Levels\\test0.txt");
        rayCaster.init(player);


        // Main loop
        while (!window.shouldClose()) {
            rayCaster.DDA(player.getPositionInfo());
            renderer.render(quad, shader, VAO);

            player.readInput();
            window.swapBuffers();
            glfwPollEvents();
        }
    }
}

namespace Projective {
    void run(GameState::Window window, Rendering::TextureQuad quad, Rendering::TextureRenderer renderer, Rendering::Shader shader, GLuint VAO) {
        // Setup Player
        Entities::PositionInfo info3D{{70, -110, 20}, {}, {}, 0, 0};
        Entities::Player3D player;
        player.updatePositionInfo(info3D);

        // Setup Raycasting logic
        GameState::True3DLogic doom(quad);
        doom.changeMap("C:\\Users\\Anderson\\CLionProjects\\CASTR\\src\\True3D Levels\\test0.txt");

        // Main loop
        while (!window.shouldClose()) {
            for (int i = 0 ; i < resolutionWidth * resolutionHeight; i++) {
                quad.textureData[i * 3 + 0] = 195;
                quad.textureData[i * 3 + 1] = 177;
                quad.textureData[i * 3 + 2] = 225;
            }

            doom.draw3D(player.getPositionInfo());
            renderer.render(quad, shader, VAO);

            // Clear Quad
            quad.clearTextureData();

            player.readInput();
            window.swapBuffers();
            glfwPollEvents();
        }
    }
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

    Projective::run(window, quad, renderer, shader, VAO);


    glDeleteVertexArrays(1, &VAO);
    // Clean up
//    glDeleteTextures(1, &textureID);
//    glDeleteBuffers(1, &VBO);
//    glDeleteProgram(shaderProgram);

    // Terminate GLFW
    glfwTerminate();

    return 0;
}
