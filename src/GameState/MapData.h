//
// Created by Anderson on 12/11/2023.
//

#ifndef CASTR_MAPDATA_H
#define CASTR_MAPDATA_H

#include "../Entities/Entity.h"

namespace GameState {
    /**
     * @brief Represents a wall with two bottom line points and a color.
     */
    struct Wall {
        std::pair<int, int> b1; ///< Bottom line point one
        std::pair<int, int> b2; ///< Bottom line point two
        int color[3];           ///< Color of the wall
    };

    /**
     * @brief Represents a sector with wall indices, heights, center, colors, and rendering properties.
     */
    struct Sector {
        std::pair<int, int> wallIdx;  ///< Indices of walls in the sector
        std::pair<int, int> heights;  ///< Bottom and top heights of the sector
        std::pair<int, int> center;   ///< Center of the sector
        int topColor[3];              ///< Color of the top surface
        int bottomColor[3];           ///< Color of the bottom surface
        int surf[800];                ///< Array for surface data
        int surface;                  ///< Indicator for the surface type
        int d;                        ///< Distance property of the sector

        /**
         * @brief Comparison function for sorting sectors based on distance.
         * @param a The first sector to compare.
         * @param b The second sector to compare.
         * @return True if the first sector is farther than the second, false otherwise.
         */
        static bool compare(Sector &a, Sector &b);
    };

    class MapData {
    public:
        static std::vector<std::string> readMapData(const std::string &filePath);
        virtual void parseMapData(const std::string &filePath) = 0;
    };

    class RayCasterMapData : public MapData {
    public:
        void parseMapData(const std::string &filePath) override;
        std::vector<std::vector<int>> map;
        Entities::PositionInfo info;
        std::size_t rows;
        std::size_t cols;
    };

    class True3DMapData : public MapData {
    public:
        void parseMapData(const std::string &filePath) override;
        Entities::PositionInfo startPos;
        std::vector<Wall> walls;         ///< Vector of walls
        std::vector<Sector> sectors;     ///< Vector of sectors
    };
}
#endif //CASTR_MAPDATA_H