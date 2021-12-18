#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HEX_SIZE 4096
#define SIZE (HEX_SIZE * 4)

struct result {
    long long num; // I assume this will be useful for part 2
    int ver; // sum of versions seen
    int len; // how long was this packet?
};

struct result munch(char *bin) {
    // grumpy
    char version = (bin[0] << 2) + (bin[1] << 1) + bin[2];
    char type = (bin[3] << 2) + (bin[4] << 1) + bin[5];
    printf("version=%d, type=%d\n", version, type);

    struct result r;
    r.num = 0;
    r.ver = version;
    r.len = 6;

    if (type == 4) {
        // literal
        int group;
        for (group = 0;; group++) {
            r.num = (r.num << 4) + (bin[r.len+group*5+1] << 3) + (bin[r.len+group*5+2] << 2) + (bin[r.len+group*5+3] << 1) + bin[r.len+group*5+4];
            if (bin[r.len+group * 5] == 0) break;
        }

        printf("last group seen = %d\n", group);
        r.len += (group + 1) * 5;
    } else {
        // operator
        char length_type = bin[6];
        r.len++;

        int psz = 0; // interpreted differently depending on length_type
        // if length_type, then next 11 bits are # packets, else next 16 bits are # bits
        for (int i = 0; i < ((length_type) ? 11 : 15); i++) {
            psz = (psz << 1) | bin[r.len++];
        }

        printf("operator, length type %d, psz=%d\n", length_type, psz);

        // read sub-packets
        int subpackets_read = 0; // interpreted differently depending on length_type
        switch (type) {
            case 0: // sum
                r.num = 0;
                break;
            case 1: // product
                r.num = 1;
                break;
            case 2: // min
                r.num = LLONG_MAX;
                break;
            case 3: // max
                r.num = LLONG_MIN;
                break;
            default:
                // no initialization for comparisons
        }


        int n_subpackets = 0;
        while (subpackets_read < psz) {
            printf("munching at %d\n", r.len);
            struct result sr = munch(bin + r.len);
            r.ver += sr.ver;
            printf("num received = %lld\n", sr.num);
            if (length_type) subpackets_read++;
            else subpackets_read += sr.len;
            r.len += sr.len;

            switch (type) {
                case 0: // sum
                    r.num += sr.num;
                    break;
                case 1: // mult
                    r.num *= sr.num;
                    break;
                case 2: // min
                    if (sr.num < r.num) r.num = sr.num;
                    break;
                case 3: // max
                    if (sr.num > r.num) r.num = sr.num;
                    break;
                case 5: // >
                    if (n_subpackets) r.num = (r.num > sr.num);
                    else r.num = sr.num;
                    break;
                case 6: // >
                    if (n_subpackets) r.num = (r.num < sr.num);
                    else r.num = sr.num;
                    break;
                case 7: // >
                    if (n_subpackets) r.num = (r.num == sr.num);
                    else r.num = sr.num;
                    break;
            }

            n_subpackets++;
        }
    }

    printf("leaving, just read %d bits\n", r.len);
    return r;
}


int main(int argc, char **argv) {

    char buf[HEX_SIZE];
    if (fgets(buf, sizeof(buf), stdin) == NULL) {
        fprintf(stderr, "failed to read buffer!!\n");
        exit(1);
    }

    // convert to "binary"
    char bin[SIZE];
    int i;
    for (i = 0; isalnum(buf[i]); i++) {
        if (isalpha(buf[i])) buf[i] = buf[i] - 'A' + 0xA;
        if (isdigit(buf[i])) buf[i] = buf[i] - '0';

        bin[i * 4 + 0] = (buf[i] & (1 << 3)) >> 3;
        bin[i * 4 + 1] = (buf[i] & (1 << 2)) >> 2;
        bin[i * 4 + 2] = (buf[i] & (1 << 1)) >> 1;
        bin[i * 4 + 3] = (buf[i] & (1 << 0)) >> 0;
    }

    printf("munching string of len=%d...\n", i);
    /*
    for (int j = 0; j < i * 4; j++) {
        printf("%d ", bin[j]);
    }
    */
    printf("\n");
    struct result res = munch(bin);

    printf("part 1 versions = %d\n", res.ver);
    printf("part 2 evaluation = %lld\n", res.num);
}
