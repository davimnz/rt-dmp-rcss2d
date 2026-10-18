#ifndef _CSV_H_
#define _CSV_H_

#include <sstream>
#include <string>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>
#include "point.h"

typedef std::vector<std::vector<std::pair<int, Point>>> GameLog;

inline int ParseLog(const std::string& filepath, GameLog& log, std::vector<int>& validCycles) {
    log.resize(22);
    for (auto& player : log) {
        player.resize(6000);
    }

    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filepath << std::endl;
        return -1;
    }
    std::stringstream buffer;
    buffer << file.rdbuf();

    /* Read lines, ignore header */
    int currentPlayer = 0;
    int currentIdx = 0;
    int currentCycle = 0;
    bool skipCycle = true;
    std::string line;
    std::getline(buffer, line);
    while (std::getline(buffer, line)) {
        std::istringstream lineStream(line);
        std::string value;

        /* Ignore columns */
        for (int j = 0; j < 14; ++j) {
            switch(j) {
                case 1:
                    std::getline(lineStream, value, ',');
                    currentCycle = std::stoi(value);
                    break;
                case 3:
                    std::getline(lineStream, value, ',');
                    if (value == "play_on") {
                        if (skipCycle) {
                            skipCycle = false;
                            validCycles.emplace_back(currentCycle);
                        }
                    }
                    else {
                        if (skipCycle == false) {
                            validCycles.emplace_back(currentCycle);
                            skipCycle = true;
                        }
                    }
                    break;
                default:
                    std::getline(lineStream, value, ',');
                    break;
            }
        }

        /* Skip non play_on cycles */
        if (skipCycle) {
            continue;
        }

        /* Read info about players */
        Point point;
        currentPlayer = 0;
        while (std::getline(lineStream, line, ',')) { /* Type (Ignore) */
            std::getline(lineStream, value, ','); /* X Position */
            point.x = std::stod(value);
            std::getline(lineStream, value, ','); /* Y Position */
            point.y = std::stod(value);
            std::getline(lineStream, value, ','); /* X Velocity */
            point.vx = std::stod(value);
            std::getline(lineStream, value, ','); /* Y Velocity */
            point.vy = std::stod(value);
            std::getline(lineStream, value, ','); /* Body */
            point.body = std::stod(value);
            std::getline(lineStream, value, ','); /* Neck (Ignore) */
            log[currentPlayer][currentIdx] = std::make_pair(currentCycle, point);
            ++currentPlayer;
        }
        ++currentIdx;
    }

    if (validCycles.size() % 2 != 0) {
        std::cerr << "Error: Number of valid cycles should be even. Check the logfile." << std::endl;
        return -1;
    }

    return currentIdx;
}

#endif
