#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 1024
// yummy global variables
long long crisk[SIZE * SIZE];
char visited[SIZE * SIZE];
int ncols = 0, nrows = 0;
int tiles;

long long min(long long x, long long y) { return (x<y)?x:y; }

void try_neighbor(int r, int c, int r1, int c1, long long *crisk, unsigned char *risk, char *visited) {
    int src = r * SIZE + c;
    int dest = r1 * SIZE + c1;
    if (visited[dest]) return;

    int risk_bonus = r1 / nrows + c1 / ncols;
    int dest_risk = (risk[(r1 % nrows) * SIZE + (c1 % ncols)] - 1 + risk_bonus) % 9 + 1;

    crisk[dest] = min(crisk[dest], crisk[src] + dest_risk);
}

int main(int argc, char **argv) {

    if (argc < 2) {
        fprintf(stderr, "usage: %s TILES\n\narguments:\n  TILES - number of times to repeat the map\n", argv[0]);
        return 1;
    }
    tiles = atoi(argv[1]);
    unsigned char risk[SIZE * SIZE];
    char buf[SIZE];
    while (fgets(buf, sizeof(buf), stdin) != NULL) {
        nrows = 0;
        for (int i = 0; buf[i] >= '0' && buf[i] <= '9'; i++) {
            risk[ncols * SIZE + nrows++] = buf[i] - '0';
        }
        ncols++;
    }

    // dijkstra's algorithm :)
    memset(crisk, 0, sizeof(crisk));
    memset(visited, 0, sizeof(visited));

    for (int i = 0; i < SIZE * SIZE; i++) {
        crisk[i] = LLONG_MAX;
    }
    crisk[0] = 0;

    int r = 0, c = 0;
    while (!visited[(nrows*tiles-1) * SIZE + ncols*tiles-1]) {
        //printf("looking at r=%d, c=%d\n", r, c);
        // look at all neighbors
        if (r > 0) try_neighbor(r, c, r-1, c, crisk, risk, visited);
        if (c > 0) try_neighbor(r, c, r, c-1, crisk, risk, visited);
        if (r < nrows * tiles - 1) try_neighbor(r, c, r+1, c, crisk, risk, visited);
        if (c < ncols * tiles - 1) try_neighbor(r, c, r, c+1, crisk, risk, visited);

        // mark visited
        visited[r * SIZE + c] = 1;

        // find next node
        int rs = 0, cs = 0, min_crisk = INT_MAX;
        for (int r1 = 0; r1 < nrows*tiles; r1++) {
            for (int c1 = 0; c1 < ncols*tiles; c1++) {
                if (!visited[r1 * SIZE + c1] && crisk[r1 * SIZE + c1] < min_crisk) {
                    //printf("next? at r=%d, c=%d score=%lld\n", r1, c1, crisk[r1 * SIZE + c1]);
                    rs = r1;
                    cs = c1;
                    min_crisk = crisk[r1 * SIZE + c1];
                }
            }
        }

        r = rs; c = cs;
    }

    printf("lowest risk path = %lld\n", crisk[(nrows*tiles - 1) * SIZE + ncols*tiles - 1]);
}
