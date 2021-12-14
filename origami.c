#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 2048

void print_map(char *map, int xmax, int ymax) {
    for (int j = 0; j <= ymax; j++) {
        for (int i = 0; i <= xmax; i++) {
            putchar((map[j * SIZE + i]) ? '#' : '.');
        }
        putchar('\n');
    }
}

int main(int argc, char **argv) {
    int x, y;
    char map[SIZE * SIZE];
    memset(map, 0, sizeof(map));

    // read map
    int xmax = 0, ymax = 0;
    while (scanf("%d,%d\n", &x, &y) == 2) {
        map[y * SIZE + x] = 1;

        if (y > ymax) ymax = y;
        if (x > xmax) xmax = x;
    }

    // read fold instructions
    char fdim;
    int floc;
    while (scanf("fold along %c=%d\n", &fdim, &floc) == 2) {

        if (fdim == 'x') {
            for (int i = 1; i <= xmax - floc; i++) {
                for (int j = 0; j <= ymax; j++) {
                    map[j * SIZE + (floc - i)] |= map[j * SIZE + (floc + i)];
                }
            }
            xmax = floc - 1;
        } else if (fdim == 'y')  {
            for (int j = 1; j <= ymax - floc; j++) {
                for (int i = 0; i <= xmax; i++) {
                    map[(floc - j) * SIZE + i] |= map[(floc + j) * SIZE + i];
                }
            }
            ymax = floc - 1;
        } else {
            fprintf(stderr, "invalid direction\n");
            return 1;
        }

        int ndots = 0;
        for (int i = 0; i <= xmax; i++) {
            for (int j = 0; j <= ymax; j++) {
                if (map[j * SIZE + i]) ndots++;
            }
        }

        printf("folding along %c=%d; now %d dots\n", fdim, floc, ndots);
    }

    print_map(map, xmax, ymax);

}
