#pragma once
#include <random>

using namespace std;

inline int randomNumber(const int high) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(0, high);
    return dist(gen);
}
