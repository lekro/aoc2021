#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 32

int try_flash(int *map, int row, int col, int rows, int cols) {
    if (row < 0 || col < 0 || row >= rows || col >= cols) return 0;
    if (map[row * SIZE + col] > 9) return 0;

    // don't bother incrementing already flashed
    map[row * SIZE + col]++;

    // leave if already flashed
    if (map[row * SIZE + col] != 10) return 0;
    int nflashes = 1;
    nflashes += try_flash(map, row-1, col, rows, cols);
    nflashes += try_flash(map, row+1, col, rows, cols);
    nflashes += try_flash(map, row, col-1, rows, cols);
    nflashes += try_flash(map, row, col+1, rows, cols);
    nflashes += try_flash(map, row-1, col-1, rows, cols);
    nflashes += try_flash(map, row+1, col+1, rows, cols);
    nflashes += try_flash(map, row+1, col-1, rows, cols);
    nflashes += try_flash(map, row-1, col+1, rows, cols);

    return nflashes;
}

void print_map(int *map, int rows, int cols) {
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            printf("%d ", map[row * SIZE + col]);
        }
        printf("\n");
    }
    printf("\n");
}


int main(int argc, char **argv) {
    int map[SIZE*SIZE];

    int rows = 0, cols = 0;

    char buf[32];
    while (fgets(buf, 32, stdin) != NULL) {
        cols = 0;
        while (1) {
            if (buf[cols] > '9' || buf[cols] < '0') break;
            map[rows * SIZE + cols] = buf[cols] - '0';
            cols++;
        }

        rows++;
    }

    int nflashes = 0;
    for (int step = 0; step < 1000; step++) {
        printf(".");
        int nflashes2 = 0;
        //print_map(map, rows, cols);
        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < cols; col++) {
                nflashes2 += try_flash(map, row, col, rows, cols);
            }
        }

        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < cols; col++) {
                if (map[row * SIZE + col] > 9) map[row * SIZE + col] = 0;
            }
        }

        nflashes += nflashes2;

        if (step == 99) {
            printf("\npart 1 nflashes = %d\n", nflashes);
        }

        if (nflashes2 == rows * cols) {
            printf("\npart 2 at round %d, they were all synchronized\n", step + 1);
            break;
        }
    }

}
