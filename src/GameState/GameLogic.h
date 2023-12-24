//
// Created by Anderson on 12/5/2023.
//

#ifndef CASTR_GAMELOGIC_H
#define CASTR_GAMELOGIC_H
#define numSect 4
#define numWall 16

#include <cstddef>
#include "../Entities/Entity.h"
#include "../Rendering/Geometry.h"
#include "MapData.h"
#include "../Entities/Player.h"
namespace GameState {
    /**
     * @brief Base class for game logic, providing pixel filling functionality.
     */
    class GameLogic {
    public:
        virtual void changeMap(const std::string &filePath) = 0;
    protected:
        /**
         * @brief Fills pixel data in the given texture quad at the specified pixel coordinates with the provided color.
         * @param quad The texture quad.
         * @param pixel The pixel coordinates.
         * @param color The color to fill.
         */
        static void fillPixels(const Rendering::TextureQuad &quad, vInt2d pixel, const int color[3]);
    };

    /**
     * @brief RayCasterLogic class for raycasting and map management.
     */
    class RayCasterLogic : public GameLogic {
    public:
        /**
         * @brief Constructor for RayCasterLogic, initializes textures based on width and height.
         * @param quad The texture quad.
         * @param texWidth The width of the textures.
         * @param texHeight The height of the textures.
         */
        explicit RayCasterLogic(Rendering::TextureQuad &quad, int texWidth, int texHeight);

        /**
         * @brief Performs the DDA (Digital Differential Analyzer) algorithm for raycasting.
         * @param positionInfo The position information of the player.
         */
        void DDA(Entities::PositionInfo positionInfo);

        /**
         * @brief Changes the map by parsing map data from the specified file path.
         * @param filePath The file path to the new map data.
         */
        void changeMap(const std::string &filePath) override;

        /**
         * @brief Initializes the game logic with player information.
         * @param player The player entity.
         */
        void init(Entities::Player &player);

    private:
        int texWidth;                 ///< Width of textures
        int texHeight;                ///< Height of textures
        Rendering::TextureQuad &quad; ///< Texture quad
        GameState::RayCasterMapData worldMap; ///< Map data
        std::vector<GLuint> texture[8];        ///< Texture vector
    };

    /**
     * @brief True3DLogic class for managing 3D rendering.
     */
    class True3DLogic : public GameLogic {
    public:
        /**
         * @brief Constructor for True3DLogic, initializes with a texture quad.
         * @param quad The texture quad.
         */
        explicit True3DLogic(Rendering::TextureQuad &quad);

        /**
         * @brief Draws the 3D scene based on player's position and angle.
         * @param positionInfo3D The 3D position information of the player.
         */
        void draw3D(Entities::PositionInfo positionInfo3D);

        /**
         * @brief Changes the map by parsing map data from the specified file path.
         * @param filePath The file path to the new map data.
         */
        void changeMap(const std::string &filePath) override;

    private:
        /**
         * @brief Projects walls onto the screen based on player's position and angle.
         * @param positionInfo3D The 3D position information of the player.
         * @param s The sector to project walls from.
         * @param angle The viewing angle.
         * @param loopNum The loop iteration number.
         */
        void projectWalls(Entities::PositionInfo positionInfo3D, Sector &s, double angle, int loopNum);

        /**
         * @brief Draws a wall onto the screen with specified screen coordinates and color.
         * @param vX X-coordinates of the wall.
         * @param vYBottom Y-coordinates of the bottom of the wall.
         * @param vYTop Y-coordinates of the top of the wall.
         * @param color The color of the wall.
         * @param s The sector containing the wall.
         */
        void drawWall(vInt2d vX, vInt2d vYBottom, vInt2d vYTop, const int color[3], Sector &s) const;

        /**
         * @brief Clips coordinates for wall rendering.
         * @param v1 First vector to clip.
         * @param v2 Second vector to clip.
         */
        static void clipCoordinates(vDouble3d &v1, vDouble3d &v2);

        Rendering::TextureQuad &quad;    ///< Texture quad
        True3DMapData mapData;
    };
}

#endif //CASTR_GAMELOGIC_H
