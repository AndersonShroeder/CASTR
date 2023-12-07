//
// Created by Anderson on 12/5/2023.
//
#include <omp.h>
#include "GameLogic.h"

namespace GameState {

    RayCasterLogic::RayCasterLogic(Rendering::TextureQuad &quad, int texWidth, int texHeight,
                                   MapData &worldMap)
            : quad(quad), texWidth(texWidth), texHeight(texHeight), worldMap(worldMap) {
        for (auto &i: texture) i.resize(texWidth * texHeight);

        //generate some textures
        for (int x = 0; x < texWidth; x++)
            for (int y = 0; y < texHeight; y++) {
                int xorcolor = (x * 256 / texWidth) ^ (y * 256 / texHeight);
                //int xcolor = x * 256 / texWidth;
                int ycolor = y * 256 / texHeight;
                int xycolor = y * 128 / texHeight + x * 128 / texWidth;
                texture[0][texWidth * y + x] =
                        65536 * 254 * (x != y && x != texWidth - y); //flat red texture with black cross
                texture[1][texWidth * y + x] = xycolor + 256 * xycolor + 65536 * xycolor; //sloped greyscale
                texture[2][texWidth * y + x] = 256 * xycolor + 65536 * xycolor; //sloped yellow gradient
                texture[3][texWidth * y + x] = xorcolor + 256 * xorcolor + 65536 * xorcolor; //xor greyscale
                texture[4][texWidth * y + x] = 256 * xorcolor; //xor green
                texture[5][texWidth * y + x] = 65536 * 192 * (x % 16 && y % 16); //red bricks
                texture[6][texWidth * y + x] = 65536 * ycolor; //red gradient
                texture[7][texWidth * y + x] = 128 + 256 * 128 + 65536 * 128; //flat grey texture
            }
    }

    void RayCasterLogic::DDA(Entities::PositionInfo2D positionInfo) {
        int screenWidth = this->quad.width;
        int screenHeight = this->quad.height;

        for (int x = 0; x < screenWidth; x++) {
            double screenXVal = 2 * x / double(screenWidth) - 1;

            vDouble2d vRayDir = (positionInfo.dir + positionInfo.plane * screenXVal);
            vDouble2d vRayUnitStepSize{(vRayDir[0] == 0) ? 1e30 : std::abs(1 / vRayDir[0]),
                                       (vRayDir[1] == 0) ? 1e30 : std::abs(1 / vRayDir[1])};
            vDouble2d vRayDist;

            vInt2d vMapPos{(int) positionInfo.pos[0], (int) positionInfo.pos[1]};
            vInt2d step;

            // Iterates through x and y and setup initial condition
            for (int i = 0; i < 2; i++) {
                step[i] = vRayDir[i] < 0 ? -1 : 1;
                vRayDist[i] =
                        vRayDir[i] < 0 ? (positionInfo.pos[i] - vMapPos[i]) : (vMapPos[i] + 1 - positionInfo.pos[i]);
                vRayDist[i] *= vRayUnitStepSize[i];
            }

            int hit = 0, side = 0, count = 0, max = 50;

            while (hit == 0 && count < max) {
                // we move down the path with the shortest distance -> this is some tom foolery with booleans:
                // if the x distance is less than y distance the bool evaluates to 0 which corresponds to x values.
                int idx = vRayDist[0] >= vRayDist[1];

                vRayDist[idx] += vRayUnitStepSize[idx];
                vMapPos[idx] += step[idx];
                side = idx;

                if (worldMap.map[vMapPos[0]][vMapPos[1]] > 0) hit = 1;
                count++;
            }

            double perpWallDist;
            if (side == 0) perpWallDist = (vRayDist[0] - vRayUnitStepSize[0]);
            else perpWallDist = (vRayDist[1] - vRayUnitStepSize[1]);

            int lineHeight = (int) screenHeight / (3 * perpWallDist);

            int drawStart = -lineHeight / 2 + screenHeight / 2;
            if (drawStart < 0)drawStart = 0;
            int drawEnd = lineHeight / 2 + screenHeight / 2;
            if (drawEnd >= screenHeight)drawEnd = screenHeight - 1;

            int r = 0, g = 0, b = 0;
            switch (worldMap.map[vMapPos[0]][vMapPos[1]]) {
                case 1:
                    r = 255;
                    break;
                case 2:
                    g = 255;
                    break;
                case 3:
                    b = 255;
                    break;
                case 4:
                    r = 255;
                    b = 255;
                    g = 255;
                    break;
                default:
                    r = 255;
                    g = 255;
                    break;
            }

            for (int y = drawStart; y < drawEnd; y++) {
                this->quad.textureData[(y * screenWidth + x) * 3] = r;
                this->quad.textureData[(y * screenWidth + x) * 3 + 1] = g;
                this->quad.textureData[(y * screenWidth + x) * 3 + 2] = b;
            }

        }
    }


    void RayCasterLogic::setMapData(MapData &mapData) {
        this->worldMap = mapData;
    }
}