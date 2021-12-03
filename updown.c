#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
    char buf[512];
    char *action, *amount_s;

    int hpos = 0, depth = 0;
    while (fgets(buf, 512, stdin) != NULL) {
        action = strtok(buf, " ");
        amount_s = strtok(NULL, "\n");

        int amount = atoi(amount_s);
        if (strcmp(action, "forward") == 0) {
            hpos += amount;
        } else if (strcmp(action, "down") == 0) {
            depth += amount;
        } else if (strcmp(action, "up") == 0) {
            depth -= amount;
        } else {
            return 1;
        }
    }

    printf("%d\n", depth * hpos);

    return 0;
}
