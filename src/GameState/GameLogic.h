//
// Created by Anderson on 12/5/2023.
//

#ifndef CASTR_GAMELOGIC_H
#define CASTR_GAMELOGIC_H


#include <cstddef>
#include "../Entities/Entity.h"
#include "../Rendering/Geometry.h"
#include "MapData.h"

namespace GameState {
    struct Mapp {
        std::vector<std::vector<int>> map;
        std::size_t rows = map.size();
        std::size_t cols = map.at(0).size();
    };

    class GameLogic {

    };

    class RayCasterLogic {
    public:
        explicit RayCasterLogic(Rendering::TextureQuad &quad, int texWidth, int texHeight);

        /**
         * @brief Performs raycasting and generates Lines for rendering.
         * @param screenWidth Width of the screen.
         * @param screenHeight Height of the screen.
         * @param positionInfo The position information of the camera.
         * @param worldMap 2D array representing the world map.
         * @return Lines object containing vertices and indices for rendering.
         */
        void DDA(Entities::PositionInfo2D positionInfo);

        void changeMap(const std::string& filePath);

    private:
        Rendering::TextureQuad &quad;
        int texWidth, texHeight;
//    TextureData textureData;
        GameState::RayCasterMapData worldMap{};
        std::vector<GLuint> texture[8];
    };

    class True3DLogic {
    public:
        explicit True3DLogic(Rendering::TextureQuad &quad);
        void draw3D(Entities::PositionInfo3D positionInfo3D);

    private:
        Rendering::TextureQuad &quad;

        int interpolate(const std::pair<int, int> &v1, const std::pair<int, int> &v2, int y);

        void drawLine(int x1, int x2, int bottomY1, int bottomY2, int topY1, int topY2);

        void clipBehindPlayer(int *x1, int *y1, int *z1, int x2, int y2, int z2);
    };
}

#endif //CASTR_GAMELOGIC_H
