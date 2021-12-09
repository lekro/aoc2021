#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>


#ifndef DEBUG
#define DEBUG 0
#endif


void print_arr(long long *fish, int len) {
    for (int i = 0; i < len; i++) printf("%lld ", fish[i]);
    printf("\n");
}

int abs(int x) { return (x > 0) ? x : -x; }

int sub1_sum(int *crabs, int ncrabs) {
    long long sum = 0;
    for (int i = 0; i < ncrabs; i++) sum += abs(crabs[i]--);
    return sum;
}

int sub1_sum2(int *crabs, int ncrabs) {
    long long sum = 0;
    for (int i = 0; i < ncrabs; i++) {
        int a = abs(crabs[i]--);
        sum += a * (a+1) / 2;
    }
    return sum;
}


int main(int argc, char **argv) {
    int crabs_orig[1024], crabs[1024];
    int ncrabs = 0;

    while (scanf("%d,", &crabs_orig[ncrabs]) == 1) {
        ncrabs++;
    }

    int last_sum = INT_MAX;
    memcpy(crabs, crabs_orig, sizeof(crabs));
    for (int c = 0; c < 2048; c++) {
        int sum = sub1_sum(crabs, ncrabs);
        if (sum > last_sum) break;
        last_sum = sum;
    }

    printf("L1 distance: %d\n", last_sum);

    last_sum = INT_MAX;
    memcpy(crabs, crabs_orig, sizeof(crabs));
    for (int c = 0; c < 2048; c++) {
        int sum = sub1_sum2(crabs, ncrabs);
        if (sum > last_sum) break;
        last_sum = sum;
    }

    printf("half L2 distance: %d\n", last_sum);


}
