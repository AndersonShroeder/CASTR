//
// Created by Anderson on 12/11/2023.
//

#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include "MapData.h"

namespace GameState {
    bool Sector::compare(Sector& a, Sector& b) {
        if (a.d > b.d) {
            return true;
        }
        return false;
    }

    std::vector<std::string> MapData::readMapData(const std::string &filePath) {
        std::ifstream inputFile(filePath);

        // Check if the file is open successfully
        if (!inputFile.is_open()) {
            std::cerr << "Error opening file: " << filePath << std::endl;
        }

        std::vector<std::string> contents;
        std::string line;
        while (std::getline(inputFile, line)) {
            contents.push_back(line);
        } inputFile.close();


        return contents;
    }

    void RayCasterMapData::parseMapData(const std::string &filePath) {
        std::vector<std::string> contents = MapData::readMapData(filePath);
        int currIdx = 0;

        #define INT_AT_INDEX(i) std::stoi(contents.at(i++))

            // Parse Map Grid
            {
                int width = INT_AT_INDEX(currIdx), height = INT_AT_INDEX(currIdx);
                int lineNumber = height + currIdx;

                // Reserve memory for the vectors
                this->map.reserve(height);
                for (; currIdx < lineNumber; currIdx++) {
                    // Reserve memory for the row vector
                    std::vector<int> row;
                    row.reserve(width);

                    for (int j = 0; j < width; j++) {
                        int value = std::stoi(contents.at(currIdx).substr(j * 2, 1));
                        row.emplace_back(value);
                    }

                    this->map.emplace_back(std::move(row));
                }
            } // Release memory


            // Parse Starting Point
            {
                double x = INT_AT_INDEX(currIdx), y = INT_AT_INDEX(currIdx);
                double dirX = INT_AT_INDEX(currIdx), dirY = INT_AT_INDEX(currIdx);

                info = {{x, y}, {dirX, dirY}, {0, .66}};
            } // Release memory

        #undef INT_AT_INDEX
    }

    void True3DMapData::parseMapData(const std::string &filePath) {
        std::vector<std::string> c = MapData::readMapData(filePath);
        std::vector<std::vector<std::string>> contents;

        for (std::string &str: c) {
            std::stringstream ss;
            ss << str;
            std::vector<std::string> result;

            while( ss.good() )
            {
                std::string substr;
                getline( ss, substr, ',' );
                result.push_back( substr );
            }

            contents.push_back(result);
        }
        int currIdx = 0;

#define GET_INT(idx, start) std::stoi(contents.at(idx).at(start))
        while (currIdx < contents.size()) {
            if (contents.at(currIdx).at(0) == "Sectors") {
                currIdx += 1;
                while(contents.at(currIdx).size() > 1 && currIdx < contents.size()) {
                    Sector st;
                    st.wallIdx = {GET_INT(currIdx, 0),GET_INT(currIdx, 1)};
                    st.heights = {GET_INT(currIdx, 2), GET_INT(currIdx, 3) - GET_INT(currIdx, 2)};
                    st.bottomColor[0] = GET_INT(currIdx, 4); st.bottomColor[1] = GET_INT(currIdx, 5); st.bottomColor[2] = GET_INT(currIdx, 6);
                    st.topColor[0] = GET_INT(currIdx, 7); st.topColor[1] = GET_INT(currIdx, 8); st.topColor[2] = GET_INT(currIdx, 9);
                    sectors.emplace_back(st);
                    currIdx += 1;
                }
            }
            else if (contents.at(currIdx).at(0) == "Walls") {
                currIdx += 1;
                while(contents.at(currIdx).size() > 1 && currIdx < contents.size()) {
                    Wall wall;
                    wall.b1 = {GET_INT(currIdx, 0),GET_INT(currIdx, 1)};
                    wall.b2 = {GET_INT(currIdx, 2), GET_INT(currIdx, 3)};
                    wall.color[0] = GET_INT(currIdx, 4); wall.color[1] = GET_INT(currIdx, 5); wall.color[2] = GET_INT(currIdx, 6);
                    walls.emplace_back(wall);
                    currIdx += 1;
                }
            }
            else currIdx += 1;
        }
#undef GET_INT
    }
}