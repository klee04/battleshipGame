#pragma once

#include "randomNumber.hpp"
#include <vector>

using namespace std;
 
inline vector<vector<int>> generateRandomBoard() {

    vector<vector<int>> shipLocations;
    vector<int> ships = {2,3,3,4,5};
    int boardDimension = 12;
 
    shipLocations.resize(boardDimension, vector<int>(boardDimension, 0));
    int shipOriginX, shipOriginY;
    for (int i = 0; i < ships.size(); i++) {
        const int shipSize = ships[i];
        int orient = randomNumber(1);
        if (orient == 0) {
   
            bool placement = false;
            while (!placement) {
                shipOriginX = randomNumber(boardDimension - shipSize - 1);
                shipOriginY = randomNumber(boardDimension - 1);
 
                bool noOverlap = true;
                for (int x = shipOriginX; x < shipOriginX+shipSize; x++) {
                    if (shipLocations[x][shipOriginY] != 0) {
                        noOverlap = false;
                    }
                }
                if (noOverlap) {
                    placement = true;
                }
            }
            for (int j = 0; j < shipSize; ++j) {
                shipLocations[shipOriginX + j][shipOriginY] = (i+1);
            }
        } else {
            bool placement = false;
            while (!placement) {
 
                shipOriginX = randomNumber(boardDimension - 1);
                shipOriginY = randomNumber(boardDimension - shipSize - 1);
 
                bool noOverlap = true;
                for (int y = shipOriginY; y < shipOriginY+shipSize; y++) {
                    if (shipLocations[shipOriginX][y] != 0) {
                        noOverlap = false;
                    }
                }
                if (noOverlap) {
                    placement = true;
                }
            }
            for (int j = 0; j < shipSize; ++j) {
                shipLocations[shipOriginX][shipOriginY + j ] = (i+1);
            }
        }
    }
    return shipLocations;
}
