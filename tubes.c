#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 1024
int x[SIZE][SIZE];
char y[SIZE][SIZE];
int rows = 0, cols = 0;

int basin_size(int row, int col) {
    if (y[row][col] || x[row][col] == 9) return 0;
    int size = 1;
    y[row][col] = 1;
    if (row > 0) size += basin_size(row-1, col);
    if (row < rows - 1) size += basin_size(row+1, col);
    if (col > 0) size += basin_size(row, col-1);
    if (col < cols - 1) size += basin_size(row, col+1);

    return size;
}


int compar(const void *a, const void *b) {
    const int *x = (const int *) a;
    const int *y = (const int *) b;

    return -(*x - *y);
}
    

int main(int argc, char **argv) {

    char buf[1024];

    while (fgets(buf, 1024, stdin) != NULL) {
        cols = 0;
        for (int i = 0; buf[i] != '\n' && buf[i] != '\0'; i++) {
            x[rows][i] = buf[i] - '0';
            cols++;
        }

        rows++;
    }

    printf("rows=%d, cols=%d\n", rows, cols);

    long long risk = 0;
    int basins[512];
    int nbasins;
    memset(y, 0, sizeof(y));
    // Search for "low points", and for each, check its basin size.
    // Note that for multiple low points in a basin, only the first
    // low point will be given a basin size > 0, because the mask
    // (stored in the global variable y) is written to by the basin_size
    // function.
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            int low_point = 1;
            low_point = low_point && (row == 0 || (x[row-1][col] > x[row][col]));
            low_point = low_point && (row == rows - 1 || (x[row+1][col] > x[row][col]));
            low_point = low_point && (col == 0 || (x[row][col-1] > x[row][col]));
            low_point = low_point && (col == cols - 1 || (x[row][col+1] > x[row][col]));
            if (low_point) risk += x[row][col] + 1;

            if (!low_point) continue;

            basins[nbasins++] = basin_size(row, col);
        }
    }

    // hooray for glibc :)
    qsort(basins, nbasins, sizeof(basins[0]), compar);

    printf("part 1 risk: %lld\n", risk);
    printf("part 2 basins: %d\n", basins[0] * basins[1] * basins[2]);


}
