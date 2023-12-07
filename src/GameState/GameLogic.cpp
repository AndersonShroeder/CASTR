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

            int lineHeight = (int) screenHeight / (perpWallDist);

            int drawStart = -lineHeight / 2 + screenHeight / 2;
            if (drawStart < 0)drawStart = 0;
            int drawEnd = lineHeight / 2 + screenHeight / 2;
            if (drawEnd >= screenHeight)drawEnd = screenHeight - 1;

            //texturing calculations
            int texNum = worldMap.map[vMapPos[0]][vMapPos[1]] - 1; //1 subtracted from it so that texture 0 can be used!

            //calculate value of wallX
            double wallX; //where exactly the wall was hit
            if (side == 0) wallX = positionInfo.pos[1] + perpWallDist * vRayDir[1];
            else wallX = positionInfo.pos[0] + perpWallDist * vRayDir[0];
            wallX -= int(wallX);

            //x coordinate on the texture
            int texX = int(wallX * double(this->texWidth));
            if (side == 0 && vRayDir[0] > 0) texX = this->texWidth - texX - 1;
            if (side == 1 && vRayDir[1] < 0) texX = this->texWidth - texX - 1;


            // How much to increase the texture coordinate per screen pixel
            double step1 = 1.0 * this->texHeight / lineHeight;
            // Starting texture coordinate
            double texPos = (drawStart - screenHeight / 2 + lineHeight / 2) * step1;
            for (int y = drawStart; y < drawEnd; y++) {
                // Cast the texture coordinate to integer, and mask with (texHeight - 1) in case of overflow
                int texY = (int) texPos & (this->texHeight - 1);
                texPos += step1;
                GLuint color = texture[texNum][this->texHeight * texY + texX];
                //make color darker for y-sides: R, G and B byte each divided through two with a "shift" and an "and"
                if(side == 1) color = (color >> 1) & 8355711;
                GLuint r = (color >> 16) & 0xFF;  // Red channel
                GLuint g = (color >> 8) & 0xFF;   // Green channel
                GLuint b = color & 0xFF;          // Blue channel
                this->quad.textureData[(y * screenWidth + x) * 3] = r;
                this->quad.textureData[(y * screenWidth + x) * 3 + 1] = g;
                this->quad.textureData[(y * screenWidth + x) * 3 + 2] = b;
            }
        }
    }


    void RayCasterLogic::setMapData(MapData &mapData) {
        this->worldMap = mapData;
    }

    True3DLogic::True3DLogic(Rendering::TextureQuad &quad) : quad(quad) {}

    void True3DLogic::draw3D(Entities::PositionInfo3D positionInfo3D) {
        double angle = (positionInfo3D.angle) * M_PI/180;
        double CS = cos(angle);
        double S = sin(angle);

        // Movement logic
        double dx = cos(angle);
        double dy = sin(angle);

        // Perspective transformation
        int x1 = 40 - positionInfo3D.pos[0];
        int y1 = 10 - positionInfo3D.pos[1];
        int x2 = 40 - positionInfo3D.pos[0];
        int y2 = 290 - positionInfo3D.pos[1];

        int wx1 = x1 * CS - y1 * S;
        int wx2 = x2 * CS - y2 * S;
        int wx3 = wx1;
        int wx4 = wx2;

        int wy1 = y1 * CS + x1 * S;
        int wy2 = y2 * CS + x2 * S;
        int wy3 = wy1;
        int wy4 = wy2;

        int wz1 = 0 - positionInfo3D.pos[2] + ((positionInfo3D.upDown * wy1) / 32.0);
        int wz2 = 0 - positionInfo3D.pos[2] + ((positionInfo3D.upDown * wy2) / 32.0);
        int wz3 = wz1 + 40;
        int wz4 = wz2 + 40;

        // Check for non-zero denominator before perspective transformation
        wy1 = (wy1 != 0) ? wy1 : 1;
        wy2 = (wy2 != 0) ? wy2 : 1;
        wy3 = (wy3 != 0) ? wy3 : 1;
        wy4 = (wy4 != 0) ? wy4 : 1;

        wx1 = static_cast<int>((wx1 * 200 / wy1) + quad.width / 2);
        wy1 = static_cast<int>((wz1 * 200 / wy1) + quad.height / 2);
        wx2 = static_cast<int>((wx2 * 200 / wy2) + quad.width / 2);
        wy2 = static_cast<int>((wz2 * 200 / wy2) + quad.height / 2);
        wx3 = static_cast<int>((wx3 * 200 / wy3) + quad.width / 2);
        wy3 = static_cast<int>((wz3 * 200 / wy3) + quad.height / 2);
        wx4 = static_cast<int>((wx4 * 200 / wy4) + quad.width / 2);
        wy4 = static_cast<int>((wz4 * 200 / wy4) + quad.height / 2);

        drawLine(wx1, wy1, wx2, wy2); // Top edge
        drawLine(wx3, wy3, wx4, wy4); // Bottom edge

        if (wx1 > 0 && wx1 < quad.width && wy1 > 0 && wy1 < quad.height) quad.textureData[(quad.height * wy1 + wx1) * 3] = 255;
        if (wx2 > 0 && wx2 < quad.width && wy2 > 0 && wy2 < quad.height) quad.textureData[(quad.height * wy2 + wx2) * 3] = 255;
        if (wx3 > 0 && wx3 < quad.width && wy3 > 0 && wy3 < quad.height) quad.textureData[(quad.height * wy3 + wx3) * 3] = 255;
        if (wx4 > 0 && wx4 < quad.width && wy4 > 0 && wy4 < quad.height) quad.textureData[(quad.height * wy4 + wx4) * 3] = 255;
    }

    void True3DLogic::drawLine(int x1, int y1, int x2, int y2) {
        // Bresenham's line drawing algorithm
        int dx = abs(x2 - x1);
        int dy = abs(y2 - y1);
        int sx = (x1 < x2) ? 1 : -1;
        int sy = (y1 < y2) ? 1 : -1;
        int err = dx - dy;

        while (true) {
            // Ensure the pixel is within the screen boundaries
            if (x1 >= 0 && x1 < quad.width && y1 >= 0 && y1 < quad.height) {
                int index = (quad.height * y1 + x1) * 3;
                if (index >= 0 && index < quad.width * quad.height * 3) {
                    quad.textureData[index] = 255;
                }
            }

            if (x1 == x2 && y1 == y2) {
                break;
            }

            int e2 = 2 * err;
            if (e2 > -dy) {
                err -= dy;
                x1 += sx;
            }
            if (e2 < dx) {
                err += dx;
                y1 += sy;
            }
        }
    }
}