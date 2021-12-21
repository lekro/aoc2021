#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAPSIZE 256

char map1[MAPSIZE * MAPSIZE * 4], map2[MAPSIZE * MAPSIZE * 4];
char algo[1024];
char buf[MAPSIZE];

int idx(int r, int c) {
    // index of (r, c) in map; (MAPSIZE, MAPSIZE) is origin.
    const int origin = MAPSIZE;
    // row-major matrix.
    int ret = (r+origin) * MAPSIZE * 2 + (c+origin);
    return ret;
}

void print_map(int *map) {
    for (int r = -MAPSIZE; r < MAPSIZE; r++) {
        for (int c = -MAPSIZE; c < MAPSIZE; c++) {
            printf("%c", (map1[idx(r, c)]) ? '#' : '.');
        }
        printf("\n");
    }
}

int main(int argc, char **argv) {

    int nrounds;
    if (argc < 2 || !(nrounds = atoi(argv[1]))) {
        fprintf(stderr, "usage: %s NROUNDS\nwhere NROUNDS is a positive integer\nexamples:\nrun part 1: %s 2\nrun part 2: %s 50\n", argv[0], argv[0], argv[0]);
        exit(1);
    }

    if (fgets(algo, 1024, stdin) == NULL) {
        fprintf(stderr, "Failed to read algorithm.");
        exit(1);
    }

    for (int i = 0; (algo[i] == '#') || (algo[i] == '.'); i++) {
        algo[i] = (algo[i] == '#') ? 1 : 0;
    }

    // read map
    int nrows = 0;
    int ncols = 0;
    memset(map1, 0, sizeof(map1));
    memset(map2, 0, sizeof(map2));
    while (fgets(buf, MAPSIZE, stdin) != NULL) {
        if ((buf[0] != '#') && (buf[0] != '.')) continue;
        for (ncols = 0; (buf[ncols] == '#') || (buf[ncols] == '.'); ncols++) {
            map1[idx(nrows, ncols)] = (buf[ncols] == '#') ? 1 : 0;
        }
        nrows++;
    }
    printf("read %d x %d matrix\n", nrows, ncols);


    char *map_new = map2;
    char *map_old = map1;

    printf("applying 'image enhancement 'algorithm' to 'image''...\n");
    for (int round = 0; round < nrounds; round++) {
        printf("[%d] ", round+1);
        for (int r = -MAPSIZE + 1; r < MAPSIZE - 1; r++) {
            for (int c = -MAPSIZE + 1; c < MAPSIZE - 1; c++) {
                // ignore edge effects :)
                int prog_idx = 0;
                for (int r2 = -1; r2 <= 1; r2++) {
                    for (int c2 = -1; c2 <= 1; c2++) {
                        prog_idx = (prog_idx << 1) | map_old[idx(r+r2, c+c2)];
                    }
                }
                assert(prog_idx < (1 << 9));
                assert(algo[prog_idx] >= 0);
                assert(algo[prog_idx] <= 1);

                map_new[idx(r, c)] = algo[prog_idx];
            }
        }

        // swap buffers
        char *tmp = map_new;
        map_new = map_old;
        map_old = tmp;
        if ((round + 1) % 10 == 0) printf("\n");
    }

    int nones = 0;
    // ignore (nrounds+1) outer layers to ignore edge effects.
    for (int r = -MAPSIZE + nrounds + 1; r < MAPSIZE - nrounds - 1; r++) {
        for (int c = -MAPSIZE + nrounds + 1; c < MAPSIZE - nrounds - 1; c++) {
            nones += map_old[idx(r, c)];
        }
    }
    printf("\n");

    printf("part 1 num ones = %d\n", nones);


}
