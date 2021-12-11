#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char flip(char x) {
    switch (x) {
        case '(':
            return ')';
        case '[':
            return ']';
        case '{':
            return '}';
        case '<':
            return '>';
        default:
            // aaaaaaa
            return -1;
    }
}


int compar(const void *a, const void *b) {
    const long long *x = (const long long *) a;
    const long long *y = (const long long *) b;

    if (*x < *y) return -1;
    if (*x == *y) return 0;
    return 1;
}

int main(int argc, char **argv) {

    char line[1024];
    char stack[1024];
    char len, stacklen, nlines = 0;

    long long score[4] = {0, 0, 0, 0};
    long long score2s[1024];
    while (fgets(line, 1024, stdin) != NULL) {
        len = strlen(line);
        stacklen = 0;

        int invalid = 0;
        long long score2 = 0;
        for (int i = 0; i < len; i++) {
            switch (line[i]) {
                case '(':
                case '[':
                case '{':
                case '<':
                    stack[stacklen++] = line[i];
                    break;
                case ')':
                    if (stack[stacklen-1] != '(' && !invalid) {
                        //puts("found invalid )");
                        score[0] += 1;
                        invalid = 1;
                    }
                    else stacklen--;
                    break;
                case ']':
                    if (stack[stacklen-1] != '[' && !invalid) {
                        //puts("found invalid ]");
                        invalid = 1;
                        score[1] += 1;
                    }
                    else stacklen--;
                    break;
                case '}':
                    if (stack[stacklen-1] != '{' && !invalid) {
                        //puts("found invalid }");
                        invalid = 1;
                        score[2] += 1;
                    }
                    else stacklen--;
                    break;
                case '>':
                    if (stack[stacklen-1] != '<' && !invalid) {
                        //puts("found invalid >");
                        invalid = 1;
                        score[3] += 1;
                    }
                    else stacklen--;
                    break;
                default:
                    goto EOL;
            }
        }
EOL:
        if (stacklen > 0 && !invalid) {
            for (int i = stacklen-1; i >= 0; i--) {
                line[stacklen-1-i] = flip(stack[i]);
            }
            line[stacklen] = '\0';

            for (int i = 0; i < stacklen; i++) {
                score2 *= 5;
                switch (line[i]) {
                    case '>':
                        score2 += 1;
                    case '}':
                        score2 += 1;
                    case ']':
                        score2 += 1;
                    case ')':
                        score2 += 1;
                        break;
                    default:
                        goto EOL2;
                }
            }
EOL2:
            //printf("completion = %s (%lld)\n", line, score2);

            score2s[nlines++] = score2;
        }
    }
    qsort(score2s, nlines, sizeof(score2s[0]), compar);

    printf("part 1 saw...  ( %lld [ %lld { %lld < %lld; score = %lld\n", score[0], score[1], score[2], score[3], score[0] * 3 + score[1] * 57 + score[2] * 1197 + score[3] * 25137);

    printf("part 2 median score = %lld\n", score2s[nlines / 2]);


}
