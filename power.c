#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
    char buf[16];
    long long dist[16];
    int numbers[1000];
    int mask = 0, oxygen = 0, carbon = 0;
    int bitlen = 0, nnumb = 0;
    memset(dist, 0, sizeof(dist));
    while (fgets(buf, 16, stdin) != NULL) {
        bitlen = 0;
        for (int i = 0; buf[i] == '0' || buf[i] == '1'; i++) {
            switch (buf[i]) {
                case '0':
                    dist[i] += -1;
                    break;
                case '1':
                    dist[i] += 1;
                    break;
                default:
                    printf("unknown char '%c'\n", buf[i]);
                    return 1;
            }
            bitlen++;
        }

        // part 2: store the number
        numbers[nnumb++] = (int) strtoul(buf, NULL, 2);
    }

    // part 1: compute
    int gamma = 0, epsilon = 0;
    for (int i = 0; buf[i] == '0' || buf[i] == '1'; i++) {
        gamma = (gamma << 1) | ((dist[i] > 0) ? 1 : 0);
        epsilon = (epsilon << 1) | ((dist[i] <= 0) ? 1 : 0);
    }

    printf("eps=%d gamma=%d power=%d\n", epsilon, gamma, epsilon * gamma);

    // part 2: filter through. we need exactly bitlen passes
    int oxy_done = 0, carb_done = 0;
    int oxy_final, carb_final;
    for (int bit = bitlen - 1; bit >= 0; bit--) {
        int oxy_vote = 0, carbon_vote = 0;
        int oxy_seen = 0, carb_seen = 0;
        for (int i = 0; i < nnumb; i++) {
            // check for numbers matching current known oxygen
            if (((numbers[i] ^ oxygen) & mask) == 0) {
                oxy_vote += ((numbers[i] & (1 << bit)) > 0) ? 1 : -1;
                oxy_final = numbers[i];
                oxy_seen += 1;
            }

            if (((numbers[i] ^ carbon) & mask) == 0) {
                carbon_vote += ((numbers[i] & (1 << bit)) > 0) ? 1 : -1;
                carb_final = numbers[i];
                carb_seen += 1;
            }
        }

        // tally votes for this bit
        if (oxy_seen > 1 && !oxy_done) oxygen |= (oxy_vote >= 0 ? 1 : 0) << bit;
        else oxy_done = 1;
        if (carb_seen > 1 && !carb_done) carbon |= (carbon_vote < 0 ? 1 : 0) << bit;
        else carb_done = 1;
        mask |= 1 << bit; // update mask to use for checking bits

    }

    printf("oxy=%d carb=%d life=%d\n", oxy_final, carb_final, oxy_final * carb_final);

    return 0;
}
