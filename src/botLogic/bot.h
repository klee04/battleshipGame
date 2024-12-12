//
// Created by Jan Karcz on 2024-09-17.
//

#ifndef BOT_H
#define BOT_H

#include <iostream>
#include <ostream>
#include <iomanip>
#include <vector>
#include <map>
#include<utility>
#include<algorithm>
#include<limits>
#include<random>

using namespace std;

class Bot {
public:
    vector<pair<int,int>> playGame(vector<vector<int>> shipLocations, int difficulty);

private:
    vector<vector<vector<int>>> boardStates;
    vector<pair<int, int>> shots;
    vector<int> ships = {2,3,3,4,5};
    vector<pair<int,int>> hitShips;
    vector<vector<int>> probBoard;
    vector<vector<pair<int,int>>> potentialShips;
    vector<pair<int,int>> hitSquares = {};
    vector<pair<int,int>> usedSquares = {};

    void initializeMap(int boardSize);
    void fillProbBoard();
    void fillProbBoard(const vector<vector<pair<int,int>>>& potentialShips);
    pair<int,int> findHighestProb(const std::vector<std::pair<int, int>>& usedValues) const;
    vector<vector<pair<int, int>>> findShips(const std::vector<std::pair<int, int>>& coordinates,const pair<int, int>& badCoordinate);

    void removeShips(pair<int,int> nextSquare);
    static vector<vector<pair<int, int>>>  removeShips(vector<vector<pair<int,int>>>, pair<int,int> nextSquare);
    void printProbBoard(int boardSize) const;
    static void printShipBoard(const vector<vector<int>> &shipLocations);
    static bool shipOnBoard(int shipVal, const vector<vector<int>>& shipLocations);
    void hitOnBoard(const vector<vector<int>>& shipLocations);
    static bool shipsLeft(const vector<vector<int>>& shipLocations);
    int randomNumber(const int high);
    pair<int,int> chooseRandom();
    pair<int,int> blurredShot();

    void hitLogic(const pair<int,int> &nextSquare, vector<vector<int>>& shipLocations, int shipVal);
};

#endif //BOT_H
