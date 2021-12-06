#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define SIZE 1024
#ifndef DEBUG
#define DEBUG 0
#endif

#ifndef PART2
#define PART2 1
#endif

int min(int x, int y) {
    return (x > y) ? y : x;
}

int max(int x, int y) {
    return (x > y) ? x : y;
}

int abs(int x) {
    return (x > 0) ? x : -x;
}

void print_board(int *board, int actual_size) {
    for (int row = 0; row < actual_size; row++) {
        for (int col = 0; col < actual_size; col++) {
            printf("%d ", board[row * SIZE + col]);
        }
        printf("\n");
    }
    printf("\n");
}

int main(int argc, char **argv) {
    char buf[1024];
    int x0, x1, y0, y1;
    int board[SIZE * SIZE];

    memset(board, 0, sizeof(board));

    while (scanf("%d,%d -> %d,%d\n", &x0, &y0, &x1, &y1) == 4) {

        if (x0 == x1) {
            // vertical line
            for (int y = min(y0, y1); y <= max(y0, y1); y++) {
                board[y * SIZE + x0]++;
            }
        } else if (y0 == y1) {
            // horizontal line
            for (int x = min(x0, x1); x <= max(x0, x1); x++) {
                board[y0 * SIZE + x]++;
            }
        } else if (abs(x1-x0) == abs(y1-y0)) {
            // 45 degrees
#if PART2
            int xdir = (x1 > x0) ? 1 : -1;
            int ydir = (y1 > y0) ? 1 : -1;
            for (int z = 0; z <= abs(x1-x0); z++) {
                board[(y0 + z * ydir) * SIZE + x0 + z * xdir]++;
            }
#endif
        }

    }

    int score1 = 0;
    for (int i = 0; i < SIZE * SIZE; i++) {
        if (board[i] > 1) score1++;
    }

    printf("score: %d\n", score1);
}
