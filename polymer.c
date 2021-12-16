#include <math.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {

    if (argc < 2) {
        fprintf(stderr, "usage: %s ITERATIONS\n", argv[0]);
        return 1;
    }
    char polymer[1 << 20], polymer1[1 << 20];
    char rules[3 * 256];
    int len, nrules = 0;
    char a, b, c;

    if (fgets(polymer, sizeof(polymer), stdin) == NULL) {
        fprintf(stderr, "couldn't read initial polymer!\n");
        return 1;
    }

    len = strlen(polymer) - 1;
    polymer[len] = '\0';

    // skip newline
    fseek(stdin, 1, SEEK_CUR);
    puts(polymer);

    while (scanf("%c%c -> %c\n", &a, &b, &c) == 3) {
        //printf("read rule: putting %c between %c and %c\n", c, a, b);
        rules[nrules*3] = a;
        rules[nrules*3+1] = b;
        rules[nrules*3+2] = c;
        nrules++;
    }


    // two buffers :)
    // define BRUTE_FORCE to use the brute force method.
    // it gets very slow after ~20 iterations.
#ifdef BRUTE_FORCE
    char *poly1 = polymer, *poly2 = polymer1;
    for (int round = 0; round < 10; round++) {
        memcpy(poly2, poly1, len);

        int nplaced = 0;
        for (int i = 0; i < len - 1; i++) {
            for (int rule = 0; rule < nrules; rule++) {
                a = rules[rule*3];
                b = rules[rule*3+1];
                c = rules[rule*3+2];

                if (!(poly1[i] == a && poly1[i+1] == b)) continue;
                //printf("found match of rule %c%c -> %c at %d\n", a, b, c, i);
                memcpy(&poly2[i+nplaced+2], &poly1[i+1], len - i);
                if (nplaced == 0) memcpy(poly2, poly1, i);
                poly2[i+nplaced+1] = c;
                nplaced++;
            }
        }
        len += nplaced;
        poly2[len] = '\0';
        //printf("%s (%d)\n", poly2, len);
        char *tmp = poly1;
        poly1 = poly2;
        poly2 = tmp;
    }

    // poly1 has the latest value

    long hist[256]; // index is ascii :)
    memset(hist, 0, sizeof(hist));
    for (int i = 0; i < len; i++) hist[poly1[i]]++;

    int max = 0, min = INT_MAX;
    for (int c = 0; c < 256; c++) {
        if (hist[c] == 0) continue;
        if (hist[c] < min) min = hist[c];
        if (hist[c] > max) max = hist[c];
    }

    printf("part 1: %d - %d = %d\n", max, min, max - min);
#else

    // ok, forget that brute force stuff
    long long key_freq[256], next_key_freq[256];
    long long val_freq[256];

    // initial key freq
    for (int i = 0; i < len - 1; i++) {
        for (int rule = 0; rule < nrules; rule++) {
            a = rules[rule*3];
            b = rules[rule*3+1];
            c = rules[rule*3+2];

            if (!(polymer[i] == a && polymer[i+1] == b)) continue;
            key_freq[rule]++;
        }

        val_freq[polymer[i]]++;
    }

    val_freq[polymer[len-1]]++;

    for (int round = 0; round < atoi(argv[1]); round++) {
        //printf("ROUND %d\n", round+1);
        memset(next_key_freq, 0, sizeof(next_key_freq));
        for (int rule = 0; rule < nrules; rule++) {

            a = rules[rule*3];
            b = rules[rule*3+1];
            c = rules[rule*3+2];
            //printf("key freq of %c%c = %lld\n", a, b, key_freq[rule]);

            char a2, b2, c2;
            for (int rule2 = 0; rule2 < nrules; rule2++) {
                a2 = rules[rule2*3];
                b2 = rules[rule2*3+1];
                if ((a == a2 && c == b2) || (c == a2 && b == b2)) {
                    next_key_freq[rule2] += key_freq[rule];
                    //printf("applying %c%c -> %c gives us %c%c\n", a, b, c, a2, b2);
                }
            }
                
            val_freq[c] += key_freq[rule];
        }
        memcpy(key_freq, next_key_freq, sizeof(next_key_freq));
    }

    long long max = 0, min = LLONG_MAX;
    for (int c = 0; c < 256; c++) {
        if (val_freq[c] == 0) continue;
        if (val_freq[c] < min) min = val_freq[c];
        if (val_freq[c] > max) max = val_freq[c];
    }

    printf("part 1: %lld - %lld = %lld\n", max, min, max - min);
#endif
}
