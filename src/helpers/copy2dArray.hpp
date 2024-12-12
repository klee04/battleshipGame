#pragma once

inline void copy2dArray(int a[12][12], int b[12][12]) {
    for (int x=0 ; x < 12; x++) {
        for (int y=0 ; y < 12; y++) {
            b[x][y] = a[x][y];
        }
    }
}
