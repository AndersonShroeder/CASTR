//
// Created by Anderson on 12/5/2023.
//

#ifndef CASTR_GAMELOGIC_H
#define CASTR_GAMELOGIC_H


#include <cstddef>
#include "../Entities/Entity.h"
#include "../Rendering/Geometry.h"

namespace GameState {
    struct MapData {
        std::vector<std::vector<int>> map;
        std::size_t rows = map.size();
        std::size_t cols = map.at(0).size();
    };

    class RayCasterLogic {
    public:
        explicit RayCasterLogic(Rendering::TextureQuad &quad, int texWidth, int texHeight,
                                MapData &worldMap);

        /**
         * @brief Performs raycasting and generates Lines for rendering.
         * @param screenWidth Width of the screen.
         * @param screenHeight Height of the screen.
         * @param positionInfo The position information of the camera.
         * @param worldMap 2D array representing the world map.
         * @return Lines object containing vertices and indices for rendering.
         */
        void DDA(Entities::PositionInfo2D positionInfo);

        void setMapData(MapData &mapData);

    private:
        Rendering::TextureQuad &quad;
        int texWidth, texHeight;
//    TextureData textureData;
        MapData &worldMap;
        std::vector<GLuint> texture[8];
    };

    class True3DLogic {
    public:
        explicit True3DLogic(Rendering::TextureQuad &quad);
        void draw3D(Entities::PositionInfo3D positionInfo3D);

    private:
        Rendering::TextureQuad &quad;

        int interpolate(const std::pair<int, int> &v1, const std::pair<int, int> &v2, int y);

        void drawLine(int x1, int y1, int x2, int y2);
    };
}

#endif //CASTR_GAMELOGIC_H
