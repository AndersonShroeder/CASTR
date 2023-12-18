//
// Created by Anderson on 12/11/2023.
//

#include <fstream>
#include <iostream>
#include <vector>
#include "MapData.h"

namespace GameState {
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
}