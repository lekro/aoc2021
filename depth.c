#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
    int lastlast = -1;
    int last = -1;
    int lastsum = -1;
    int curr;
    int count = 0, count2 = 0;
    while (scanf("%d\n", &curr) != EOF) {
        if (last != -1 && curr > last) count++;
        if (last != -1 && lastlast != -1 && lastsum != -1 && curr + last + lastlast > lastsum) {
            count2++;
        }

        if (lastlast != -1) {
            lastsum = curr + last + lastlast;
        }
        lastlast = last;
        last = curr;
    }

    printf("%d\n", count);
    printf("%d\n", count2);
}
