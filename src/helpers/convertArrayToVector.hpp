#pragma once

#include <vector>

using namespace std;

inline vector<vector<int>> convertArrayToVector(int a[12][12]) {
   vector<vector<int>> finalVector;

   for (int r = 0; r < 12; ++r) {
      vector<int> rowVector;

      for (int c = 0; c < 12; ++c) {
         rowVector.push_back(a[r][c]);
      }

      finalVector.push_back(rowVector);
   }

   return finalVector;
}
