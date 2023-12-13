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

        // Parse Map Grid
        int width = std::stoi(contents.at(1));
        int height = std::stoi(contents.at(2));

        for (int i = 3; i < height + 3; i++) {
            std::vector<int> row;
            for (int j = 0; j < width; j++) {
                row.push_back(std::stoi(contents.at(i).substr(j*2, j*2+1)));
            }
            this->map.push_back(row);
        }

        this->rows = height;
        this->cols = width;

        // Parse Starting Point
    }
}