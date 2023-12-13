//
// Created by Anderson on 12/11/2023.
//

#ifndef CASTR_MAPDATA_H
#define CASTR_MAPDATA_H

namespace GameState {
    class MapData {
    public:
        static std::vector<std::string> readMapData(const std::string &filePath);
        virtual void parseMapData(const std::string &filePath) = 0;

    private:

    };

    class RayCasterMapData : public MapData {
    public:
        void parseMapData(const std::string &filePath) override;
        std::vector<std::vector<int>> map;
        std::size_t rows;
        std::size_t cols;
    };
}
#endif //CASTR_MAPDATA_H