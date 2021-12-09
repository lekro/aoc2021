#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int bitmask_to_num(char x) {
    switch ((int) x & 0x7f) {
        case 0x77: return 0;
        case 0x24: return 1;
        case 0x5d: return 2;
        case 0x6d: return 3;
        case 0x2e: return 4;
        case 0x6b: return 5;
        case 0x7b: return 6;
        case 0x25: return 7;
        case 0x7f: return 8;
        case 0x6f: return 9;
        default:
           fprintf(stderr, "UNKNOWN BITMASK %x\n", (int) x & 0x7f);
           exit(1);
    }
}

int main(int argc, char **argv) {

    char buf[1024];
    char train[10][1024];
    char test[4][1024];

    int total_small = 0;
    long long total_big = 0;
    while (fgets(buf, 1024, stdin) != NULL) {

        // read train digits
        char *x = strtok(buf, " |\n");
        strcpy(train[0], x);

        for (int i = 1; i < 10; i++) {
            x = strtok(NULL, " |\n");
            strcpy(train[i], x);
        }

        for (int i = 0; i < 4; i++) {
            x = strtok(NULL, " |\n");
            strcpy(test[i], x);
        }

        // part 1
        for (int i = 0; i < 4; i++) {
            switch (strlen(test[i])) {
                case 2:
                case 3:
                case 4:
                case 7:
                    total_small++;
                default:
            }
        }

        // part 2
        // Suppose we have some "black box" function, f, that permutes
        // the input. Using specially crafted examples, we will see what
        // permutes to what, and slowly whittle down on the
        // possibilities for what f maps a certain value to.
        // Note that if bit i is 0 in possible[j], that indicates that
        // input j must not map to output i.
        char possible[7] = { 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f };


        // The actual examples needed are only 1, 7, 4, 9, 6, 0, and the
        // others are redundant.
        //
        // here be spaghetti

        // 1
        char mask = 0;
        for (int i = 0; i < 10; i++) {
            if (strlen(train[i]) != 2) continue;
            for (int j = 0; j < 2; j++) {
                mask |= 1 << (train[i][j] - 'a');
            }
            //printf("%s %x %x\n", train[i], possible[4], mask);

            possible[0] &= ~mask;
            possible[1] &= ~mask;
            possible[2] &= mask;
            possible[3] &= ~mask;
            possible[4] &= ~mask;
            possible[5] &= mask;
            possible[6] &= ~mask;
            break;
        }

        // 7
        mask = 0;
        for (int i = 0; i < 10; i++) {
            if (strlen(train[i]) != 3) continue;
            for (int j = 0; j < 3; j++) {
                mask |= 1 << (train[i][j] - 'a');
            }
            //printf("%s %x %x\n", train[i], possible[4], mask);

            possible[0] &= mask;
            possible[1] &= ~mask;
            possible[2] &= mask;
            possible[3] &= ~mask;
            possible[4] &= ~mask;
            possible[5] &= mask;
            possible[6] &= ~mask;
            break;
        }

        // we are now certain about possible[0]

        // 4
        mask = 0;
        for (int i = 0; i < 10; i++) {
            if (strlen(train[i]) != 4) continue;
            for (int j = 0; j < 4; j++) {
                mask |= 1 << (train[i][j] - 'a');
            }
            //printf("%s %x %x\n", train[i], possible[4], mask);

            possible[0] &= ~mask;
            possible[1] &= mask;
            possible[2] &= mask;
            possible[3] &= mask;
            possible[4] &= ~mask;
            possible[5] &= mask;
            possible[6] &= ~mask;
            break;
        }

        // 9
        for (int i = 0; i < 10; i++) {
            if (strlen(train[i]) != 6) continue;
            mask = 0;
            for (int j = 0; j < 6; j++) {
                mask |= 1 << (train[i][j] - 'a');
            }

            char query = possible[1] | possible[2] | possible[3] | possible[5];
            if ((mask & query) != query) continue;
            //printf("%s %x %x\n", train[i], possible[4], mask);

            possible[0] &= mask;
            possible[1] &= mask;
            possible[2] &= mask;
            possible[3] &= mask;
            possible[4] &= ~mask;
            possible[5] &= mask;
            possible[6] &= mask;
            break;
        }

        // we are now certain about possible[4] and possible[6]

        // 6
        for (int i = 0; i < 10; i++) {
            if (strlen(train[i]) != 6) continue;
            mask = 0;
            for (int j = 0; j < 6; j++) {
                mask |= 1 << (train[i][j] - 'a');
            }

            char query = possible[1] | possible[6] | possible[3] | possible[4];
            if ((mask & query) != query) continue;
            //printf("%s\n", train[i]);

            possible[0] &= mask;
            possible[1] &= mask;
            possible[2] &= ~mask;
            possible[3] &= mask;
            possible[4] &= mask;
            possible[5] &= mask;
            possible[6] &= mask;
            break;
        }

        // we are now certain about possible[2] and possible[5]
        // 0
        for (int i = 0; i < 10; i++) {
            if (strlen(train[i]) != 6) continue;
            mask = 0;
            for (int j = 0; j < 6; j++) {
                mask |= 1 << (train[i][j] - 'a');
            }

            char query = possible[0] | possible[2] | possible[5] | possible[6] | possible[4];
            if ((mask & query) != query) continue;
            //printf("%s\n", train[i]);

            possible[0] &= mask;
            possible[1] &= mask;
            possible[2] &= mask;
            possible[3] &= ~mask;
            possible[4] &= mask;
            possible[5] &= mask;
            possible[6] &= mask;
            break;
        }

        // we are now certain about possible[1] and possible[3]

        // printf("%x %x %x %x %x %x %x\n", possible[0], possible[1], possible[2], possible[3], possible[4], possible[5], possible[6]);

        // done with deduction; let's see what the numbers were
        int realnum = 0;
        for (int i = 0; i < 4; i++) {
            realnum *= 10;
            char actual = 0;
            for (int j = 0; j < strlen(test[i]); j++) {
                char mask = 1 << (test[i][j] - 'a');

                for (int k = 0; k < 10; k++) {
                    if (possible[k] & mask) {
                        actual |= (1 << k);
                    }
                }

            }

            realnum += bitmask_to_num(actual);
        }

        // printf("realnum %d\n", realnum);
        total_big += realnum;


    }

    printf("part 1 = %d\n", total_small);
    printf("part 2 = %lld\n", total_big);

}
