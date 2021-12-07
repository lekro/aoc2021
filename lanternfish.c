#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#ifndef DEBUG
#define DEBUG 0
#endif


#define NAGES 9
#define NDAYS 256


void print_arr(long long *fish, int len) {
    for (int i = 0; i < len; i++) printf("%lld ", fish[i]);
    printf("\n");
}


int main(int argc, char **argv) {
    char buf[1024];
    int read_fish;
    long long fish[NAGES], next_fish[NAGES];

    memset(fish, 0, sizeof(fish));

    while (scanf("%d,", &read_fish) == 1) {

        fish[read_fish]++;
        printf("read %d\n", read_fish);

    }

    for (int t = 0; t < NDAYS; t++) {
        print_arr(fish, NAGES);
        // handle new lanternfish
        memset(next_fish, 0, sizeof(next_fish));
        next_fish[NAGES - 1] = fish[0];
        next_fish[NAGES - 3] = fish[0];

        // handle aging
        for (int i = 1; i < NAGES; i++) {
            next_fish[i - 1] += fish[i];
        }

        memcpy(fish, next_fish, sizeof(fish));
    }

    long long total_fish = 0;
    for (int i = 0; i < NAGES; i++) {
        total_fish += fish[i];
    }
    printf("total fish = %lld\n", total_fish);

}
