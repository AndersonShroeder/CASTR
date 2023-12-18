//
// Created by Anderson on 12/5/2023.
//
#include <omp.h>
#include "GameLogic.h"
#include <algorithm>
namespace GameState {

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
                        0,  4, 0, 40, 2,3, //sector 1
                        4,  8, 0, 40, 4,5, //sector 2
                        8, 12, 0, 40, 6,7, //sector 3
                        12,16, 0, 40, 0,1, //sector 4
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
            S[s].wallIdx = {loadSectors[v1 + 0], loadSectors[v1 + 1]};
            S[s].heights = {loadSectors[v1 + 2], loadSectors[v1 + 3] - loadSectors[v1 + 2]};
            v1 += 6;

            for (w = S[s].wallIdx.first; w < S[s].wallIdx.second; w++) {
                W[w].b1 = {loadWalls[v2 + 0], loadWalls[v2 + 1]};
                W[w].b2 = {loadWalls[v2 + 2], loadWalls[v2 + 3]};
                W[w].color[0] = loadWalls[v2 + 4]; W[w].color[1] = loadWalls[v2 + 5]; W[w].color[2] = loadWalls[v2 + 6];
                v2 += 7;
            }
        }
    }

    void True3DLogic::draw3D(Entities::PositionInfo positionInfo3D) {
        double angle = (positionInfo3D.angle) * M_PI/180;
        double CS = cos(angle);
        double S = sin(angle);

//        MAGIC NUMBER 4 -> Need to fix this probably chagne to vector of Sectors
        std::sort(this->S, this->S + 4, Sector::compare);

        for (int s = 0; s < numSect; s++) {
            this->S[s].d = 0; // Clear dist
            for (int loop = 0; loop < 2; loop++) {
                for (int w = this->S[s].wallIdx.first; w < this->S[s].wallIdx.second; w++) {
                    // Perspective transformation
                    int x1 = W[w].b1[0] - positionInfo3D.pos[0];
                    int y1 = W[w].b1[1] - positionInfo3D.pos[1];
                    int x2 = W[w].b2[0] - positionInfo3D.pos[0];
                    int y2 = W[w].b2[1] - positionInfo3D.pos[1];

                    if(loop==0){ int swp=x1; x1=x2; x2=swp; swp=y1; y1=y2; y2=swp;}

                    int wx1 = x1 * CS - y1 * S;
                    int wx2 = x2 * CS - y2 * S;
                    int wx3 = wx1;
                    int wx4 = wx2;

                    int wy1 = y1 * CS + x1 * S;
                    int wy2 = y2 * CS + x2 * S;
                    int wy3 = wy1;
                    int wy4 = wy2;

                    this->S[s].d+=distance(0,0, (wx1+wx2)/2, (wy1+wy2)/2 );

                    int wz1 = this->S[s].heights.first - positionInfo3D.pos[2] + ((positionInfo3D.upDown * wy1) / 32.0);
                    int wz2 = this->S[s].heights.first - positionInfo3D.pos[2] + ((positionInfo3D.upDown * wy2) / 32.0);
                    int wz3 = wz1 + this->S[s].heights.second;
                    int wz4 = wz2 + this->S[s].heights.second;

                    // Check for non-zero denominator before perspective transformation
                    wy1 = (wy1 != 0) ? wy1 : 1;
                    wy2 = (wy2 != 0) ? wy2 : 1;
                    wy3 = (wy3 != 0) ? wy3 : 1;
                    wy4 = (wy4 != 0) ? wy4 : 1;

                    //dont draw if behind player
                    if(wy1<1 && wy2<1){ continue;}      //wall behind player, dont draw
                    //point 1 behind player, clip
                    if(wy1<1)
                    {
                        clipBehindPlayer(&wx1,&wy1,&wz1, wx2,wy2,wz2); //bottom line
                        clipBehindPlayer(&wx3,&wy3,&wz3, wx4,wy4,wz4); //top line
                    }
                    //point 2 behind player, clip
                    if(wy2<1)
                    {
                        clipBehindPlayer(&wx2,&wy2,&wz2, wx1,wy1,wz1); //bottom line
                        clipBehindPlayer(&wx4,&wy4,&wz4, wx3,wy3,wz3); //top line
                    }

                    wx1 = static_cast<int>((wx1 * 200 / (wy1)) + quad.width / 2);
                    wy1 = static_cast<int>((wz1 * 200 / (wy1)) + quad.height / 2);
                    wx2 = static_cast<int>((wx2 * 200 / (wy2)) + quad.width / 2);
                    wy2 = static_cast<int>((wz2 * 200 / (wy2)) + quad.height / 2);
                    wy3 = static_cast<int>((wz3 * 200 / (wy3)) + quad.height / 2);
                    wy4 = static_cast<int>((wz4 * 200 / (wy4)) + quad.height / 2);
                    drawLine(wx1, wx2, wy1, wy2, wy3, wy4, W[w].color); // Top edge
                }
            }
            this->S[s].d /= (this->S[s].wallIdx.second - this->S[s].wallIdx.first);
        }
    }

    void True3DLogic::clipBehindPlayer(int *x1,int *y1,int *z1, int x2,int y2,int z2) //clip line
    {
        float da=*y1;                                 //distance plane -> point a
        float db= y2;                                 //distance plane -> point b
        float d=da-db; if(d==0){ d=1;}
        float s = da/(da-db);                         //intersection factor (between 0 and 1)
        *x1 = *x1 + s*(x2-(*x1));
        *y1 = *y1 + s*(y2-(*y1)); if(*y1==0){ *y1=1;} //prevent divide by zero
        *z1 = *z1 + s*(z2-(*z1));
    }

    void True3DLogic::drawLine(int x1, int x2, int bottomY1, int bottomY2, int topY1, int topY2, int color[3]) {
        // Bresenham's line drawing algorithm
        int dx = x2 - x1; if (dx == 0) dx = 0;
        int dyb = bottomY2 - bottomY1;
        int dyt = topY2 - topY1;
        int xs = x1;

//        //CLIP X
        if(x1<   0){ x1=   0;} //clip left
        if(x2<   0){ x2=   0;} //clip left
        if(x1>quad.width){ x1=quad.width;} //clip right
        if(x2>quad.width){ x2=quad.width;} //clip right
        //draw x verticle lines
        for(int x=x1;x<x2;x++)
        {
            //The Y start and end point
            int y1 = dyb*(x-xs+0.5)/dx+bottomY1; //y bottom point
            int y2 = dyt*(x-xs+0.5)/dx+topY1; //y bottom point
            //Clip Y
            if(y1<   0){ y1=  0;} //clip y
            if(y2<   0){ y2=  0;} //clip y
            if(y1>quad.height){ y1=quad.height;} //clip y
            if(y2>quad.height){ y2=quad.height;} //clip y

            for(int y=y1;y<y2;y++){
                quad.textureData[(quad.width*y + x) * 3 + 0] = color[0];
                quad.textureData[(quad.width*y + x) * 3 + 1] = color[1];
                quad.textureData[(quad.width*y + x) * 3 + 2] = color[2];
            }
        }
    }

    int True3DLogic::distance(int x1, int y1, int x2, int y2) {
        return sqrt((x2-x1)*(x2-x1) + (y2 - y1) * (y2 - y1));
    }

    bool Sector::compare(Sector& a, Sector& b) {
        if (a.d > b.d) {
            return true;
        }
        return false;
    }
}