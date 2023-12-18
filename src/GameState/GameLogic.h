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
    template<typename T>
    struct Point{
        T x;
        T y;
    };
    struct Wall {
        vInt2d b1; // Bottom line point one
        vInt2d b2; // Bottom line point two
        int color[3];
    };

    struct Sector {
        std::pair<int, int> wallIdx;
        std::pair<int, int> heights; // bottom/top heights
        std::pair<int, int> center; // Center of sector
        int d;

        static bool compare(Sector &a, Sector &b);
    };


    class GameLogic {
    };

    class RayCasterLogic : public GameLogic{
    public:
        explicit RayCasterLogic(Rendering::TextureQuad &quad, int texWidth, int texHeight);

        void DDA(Entities::PositionInfo positionInfo);

        void changeMap(const std::string& filePath);

        void init(Entities::Player &player);

    private:
        Rendering::TextureQuad &quad;
        int texWidth, texHeight;
        GameState::RayCasterMapData worldMap;
        std::vector<GLuint> texture[8];
    };

    class True3DLogic : public GameLogic{
    public:
        explicit True3DLogic(Rendering::TextureQuad &quad);
        void draw3D(Entities::PositionInfo positionInfo3D);

    private:
        Rendering::TextureQuad &quad;

        int distance(int x1, int y1, int x2, int y2);

        void drawLine(int x1, int x2, int bottomY1, int bottomY2, int topY1, int topY2, int color[3]);

        void clipBehindPlayer(int *x1, int *y1, int *z1, int x2, int y2, int z2);

        Wall W[30];
        Sector S[30];
    };
}

#endif //CASTR_GAMELOGIC_H
