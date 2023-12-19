//
// Created by Anderson on 12/5/2023.
//
#include <omp.h>
#include "GameLogic.h"
#include <algorithm>
namespace GameState {
    void GameLogic::fillPixels(const Rendering::TextureQuad& quad, vInt2d pixel, const int *color) {
        int y = pixel[1], x = pixel[0];
        quad.textureData[(quad.width*y + x) * 3 + 0] = color[0];
        quad.textureData[(quad.width*y + x) * 3 + 1] = color[1];
        quad.textureData[(quad.width*y + x) * 3 + 2] = color[2];
    }

    RayCasterLogic::RayCasterLogic(Rendering::TextureQuad &quad, int texWidth, int texHeight)
            : quad(quad), texWidth(texWidth), texHeight(texHeight) {
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

    void RayCasterLogic::DDA(Entities::PositionInfo positionInfo) {
        int screenWidth = this->quad.width;
        int screenHeight = this->quad.height;

        for (int i = 0 ; i < screenWidth * screenHeight; i++) {
            quad.textureData[i * 3 + 2] = 255;
        }

        for (int x = 0; x < screenWidth; x++) {
            double screenXVal = 2 * x / double(screenWidth) - 1;

            vDouble3d vRayDir = (positionInfo.dir + positionInfo.plane * screenXVal);
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

    void RayCasterLogic::changeMap(const std::string &filePath) {
        this->worldMap.parseMapData(filePath);
    }

    void RayCasterLogic::init(Entities::Player &player) {
        player.updatePositionInfo(this->worldMap.info);
    }
    True3DLogic::True3DLogic(Rendering::TextureQuad &quad) : quad(quad) {
        int loadSectors[]=
                {//wall start, wall end, z1 height, z2 height, bottom color, top color
                        0,  4, 0, 40, 255, 0, 0, 0, 255, 0, //sector 1
                        4,  8, 0, 40, 255, 0, 0, 0, 255, 0, //sector 2
                        8, 12, 0, 40,  255, 0, 0, 0, 255, 0, //sector 3
                        12,16, 0, 40,  255, 0, 0, 0, 255, 0, //sector 4
                };

        int loadWalls[]=
                {//x1,y1, x2,y2, color
                        0, 0, 32, 0, 0, 177, 225,
                        32, 0, 32,32, 0, 177/2, 225/2,
                        32,32,  0,32, 0, 177, 225,
                        0,32,  0, 0, 0, 177/2, 225/2,

                        64, 0, 96, 0, 0, 177, 225,
                        96, 0, 96,32, 0, 177/2, 225/2,
                        96,32, 64,32, 0, 177, 225,
                        64,32, 64, 0, 0, 177/2, 225/2,

                        64, 64, 96, 64, 0, 177, 225,
                        96, 64, 96, 96, 0, 177/2, 225/2,
                        96, 96, 64, 96, 0, 177, 225,
                        64, 96, 64, 64, 0, 177/2, 225/2,

                        0, 64, 32, 64, 0, 177, 225,
                        32, 64, 32, 96, 0, 177/2, 225/2,
                        32, 96,  0, 96, 0, 177, 225,
                        0, 96,  0, 64, 0, 177/2, 225/2,
                };

        int v1 = 0, v2 = 0, w;
        for (int s = 0; s < numSect; s++) {
            Sector st;
            st.wallIdx = {loadSectors[v1 + 0], loadSectors[v1 + 1]};
            st.heights = {loadSectors[v1 + 2], loadSectors[v1 + 3] - loadSectors[v1 + 2]};
            st.bottomColor[0] = loadSectors[v1+4]; st.bottomColor[1] = loadSectors[v1+5]; st.bottomColor[2] = loadSectors[v1+6];
            st.topColor[0] = loadSectors[v1+7]; st.topColor[1] = loadSectors[v1+8]; st.topColor[2] = loadSectors[v1+9];
            mapData.sectors.emplace_back(st);
            v1 += 10;

            for (w = st.wallIdx.first; w < st.wallIdx.second; w++) {
                Wall wall;
                wall.b1 = {loadWalls[v2 + 0], loadWalls[v2 + 1]};
                wall.b2 = {loadWalls[v2 + 2], loadWalls[v2 + 3]};
                wall.color[0] = loadWalls[v2 + 4]; wall.color[1] = loadWalls[v2 + 5]; wall.color[2] = loadWalls[v2 + 6];
                mapData.walls.emplace_back(wall);
                v2 += 7;
            }
        }
    }

    void True3DLogic::projectWalls(Entities::PositionInfo positionInfo3D, Sector &s, double angle, int loopNum) {
        for (int w = s.wallIdx.first; w < s.wallIdx.second; w++) {
            vDouble3d p1 = {static_cast<double>(mapData.walls.at(w).b1.first), static_cast<double>(mapData.walls.at(w).b1.second)};
            vDouble3d p2 = {static_cast<double>(mapData.walls.at(w).b2.first), static_cast<double>(mapData.walls.at(w).b2.second)};
            p1 -= positionInfo3D.pos; p2 -= positionInfo3D.pos;

            if (loopNum == 0) {
                std::swap(p1[0], p2[0]);
                std::swap(p1[1], p2[1]);
            }

            // World points
            vDouble3d wp1 = rotate2D(p1, angle);
            vDouble3d wp2 = rotate2D(p2, angle);
            vDouble3d wp3 = wp1;
            vDouble3d wp4 = wp2;

            // While z-component of wp1 and wp2 is zero, magnitude of the average is distance!
            vDouble3d sum = (wp1 + wp2) * 1/2;
            s.d += static_cast<int>(sum.magnitude());

            // Calculate world Z values
            wp1[2] = s.heights.first - positionInfo3D.pos[2] + ((positionInfo3D.upDown * wp1[1]) / 32.0);
            wp2[2] = s.heights.first - positionInfo3D.pos[2] + ((positionInfo3D.upDown * wp2[1]) / 32.0);
            wp3[2] = wp1[2] + s.heights.second;
            wp4[2] = wp2[2] + s.heights.second;

            // wall behind player, don't draw
            if(wp1[1]<1 && wp2[1]<1) continue;

            // Otherwise check if wall needs to be clipped to avoid rendering enigmas
            if(wp1[1]<1)
            {
                True3DLogic::clipCoordinates(wp1,wp2); //bottom line
                True3DLogic::clipCoordinates(wp3,wp4); //top line
            }

            if(wp2[1]<1)
            {
                True3DLogic::clipCoordinates(wp2,wp1); //bottom line
                True3DLogic::clipCoordinates(wp4, wp3); //top line
            }

            // Project world points onto screen
            int wx1 = static_cast<int>((wp1[0] * 200 / (wp1[1]) + quad.width / 2));
            int wy1 = static_cast<int>((wp1[2] * 200 / (wp1[1]) + quad.height / 2));
            int wx2 = static_cast<int>((wp2[0] * 200 / (wp2[1]) + quad.width / 2));
            int wy2 = static_cast<int>((wp2[2] * 200 / (wp2[1]) + quad.height / 2));
            int wy3 = static_cast<int>((wp3[2] * 200 / (wp3[1]) + quad.height / 2));
            int wy4 = static_cast<int>((wp4[2] * 200 / (wp4[1]) + quad.height / 2));

            // Draw Wall onto screen with screen coordinates
            drawWall({wx1, wx2}, {wy1, wy2}, {wy3, wy4}, mapData.walls[w].color, s);
        }
        s.d /= (s.wallIdx.second - s.wallIdx.first);
        s.surface *= -1;
    }

    void True3DLogic::draw3D(Entities::PositionInfo positionInfo3D) {
        // Player angle in radians
        const double angle = (positionInfo3D.angle) * M_PI/180;

        // Sort Sectors by distance for rendering purposes
        std::sort(std::begin(mapData.sectors), std::end(mapData.sectors), Sector::compare);

        for (Sector &s : mapData.sectors) {
            s.d = 0; // Clear dist

            // Determine if drawing top, bottom or neither for sector
            s.surface = positionInfo3D.pos[2] < s.heights.first ? 1 : positionInfo3D.pos[2] > s.heights.second ? 2: 0;

            projectWalls(positionInfo3D, s, angle, 0); // Back Walls
            projectWalls(positionInfo3D, s, angle, 1); // Front Walls
        }
    }

    void True3DLogic::clipCoordinates(vDouble3d &v1, vDouble3d &v2)
    {
        double da = v1[1];
        double db = v2[1];
        double s = da / (da - db);

        // Modify ending points to render such that appropriately clipped
        v1[0] = static_cast<int>(v1[0] + s * (v2[0] - v1[0]));
        v1[1] = static_cast<int>(v1[1] + s * (v2[1] - v1[1])); if (v1[1] == 0) { v1[1] = 1; }
        v1[2] = static_cast<int>(v1[2] + s * (v2[2] - v1[2]));
    }

    void True3DLogic::drawWall(vInt2d vX, vInt2d vYBottom, vInt2d vYTop, const int color[3], Sector &s) const {
        int dx = vX[1] - vX[0]; if (dx == 0) dx = 0;
        int dyb = vYBottom[1] - vYBottom[0];
        int dyt = vYTop[1] - vYTop[0];
        int xs = vX[0];

        // Only render points that appear on screen
        vX[0] = vX[0] < 0 ? 0 : vX[0] > quad.width ? quad.width : vX[0];
        vX[1] = vX[1] < 0 ? 0 : vX[1] > quad.width ? quad.width : vX[1];

        for(int x=vX[0];x<vX[1];x++) {
            //The Y start and end point
            vDouble2d vY = {dyb * (x - xs + 0.5) / dx + vYBottom[0], dyt * (x - xs + 0.5) / dx + vYTop[0]}; //y bottom point
            int y1 = vY[0] < 0 ? 0 : vY[0] > quad.height ? quad.height : static_cast<int>(vY[0]);
            int y2 = vY[1] < 0 ? 0 : vY[1] > quad.height ? quad.height : static_cast<int>(vY[1]);

            if(s.surface== 1){s.surf[x]=y1; continue;}
            if(s.surface== 2){s.surf[x]=y2; continue;}

            switch(s.surface) {
                // Draw bottom surface if visible
                case -1:
                    for(int y = s.surf[x]; y < y1; y++) GameLogic::fillPixels(quad, {x, y}, s.bottomColor);


                // Draw top surface if visible
                case -2:
                    for(int y = y2; y < s.surf[x]; y++) GameLogic::fillPixels(quad, {x, y}, s.topColor);


                // Always draw wall
                default:
                    for(int y = y1; y < y2; y++) GameLogic::fillPixels(quad, {x, y}, color);
            }
        }
    }
}