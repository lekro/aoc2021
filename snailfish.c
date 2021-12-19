#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// this problem is terrible.

struct num {
    char literal; // 1 if literal, 0 otherwise
    struct num *left, *right;
    long long val;
};

enum exp_status { EXP_R, EXP_L, EXP_DONE, EXP_NONE };

struct exp {
    enum exp_status exploded;
    long long val;
};

long long magnitude(struct num *z) {
    if (z->literal) {
#ifdef DEBUG
        printf("%lld", z->val);
#endif
        return z->val;
    }

    long long x, y;
#ifdef DEBUG
    printf("[");
#endif
    x = 3 * magnitude(z->left);
#ifdef DEBUG
    printf(",");
#endif
    y = 2 * magnitude(z->right);
#ifdef DEBUG
    printf("]");
#endif

    return x+y;
}


struct num *parse_num(const char **buf) {
    struct num *n = malloc(sizeof(*n));
    if (isdigit(**buf)) {
        // this is a constant
        char *endptr;
        long long val = strtoll(*buf, &endptr, 10);
        *buf = endptr;
        n->literal = 1;
        n->left = NULL;
        n->right = NULL;
        n->val = val;
        return n;
    } else if (**buf == '[') {
        (*buf)++; // skip [
        n->literal = 0;
        n->left = parse_num(buf);
        (*buf)++; // skip ,
        n->right = parse_num(buf);
        (*buf)++; // skip ]
        return n;
    } else {
        fprintf(stderr, "invalid character in expression '%c'\n", **buf);
        exit(1);
        return NULL;
    }
}

void send_explosion(struct num *z, long long val, enum exp_status dir) {
    if (z->literal) z->val += val;
    else send_explosion((dir == EXP_L) ? z->left : z->right, val, dir);
}

struct exp explode(struct num *z, int depth) {

    struct exp ret;
    ret.exploded = EXP_NONE;
    if (z->literal) return ret;
    if (depth == 4) {
        // check if either side is a pair
        if (!z->left->literal) {
            // assuming right is a literal
            ret.exploded = EXP_L;
            send_explosion(z->right, z->left->right->val, EXP_L);
            ret.val = z->left->left->val;
            free(z->left->left);
            free(z->left->right);
            z->left->literal = 1;
            z->left->val = 0;
        } else if (!z->right->literal) {
            ret.exploded = EXP_R;
            send_explosion(z->left, z->right->left->val, EXP_R);
            ret.val = z->right->right->val;
            free(z->right->left);
            free(z->right->right);
            z->right->literal = 1;
            z->right->val = 0;
        } else {
            // nothing to do
            ret.exploded = EXP_NONE;
        }
    } else {
        // check left child
        ret = explode(z->left, depth+1);
        // if done, quit. if need to go left, we can't service that here.
        if (ret.exploded == EXP_DONE || ret.exploded == EXP_L) return ret;
        if (ret.exploded == EXP_R) {
            // we can forward this over to the leftmost node in the right now!
            send_explosion(z->right, ret.val, EXP_L);
            ret.exploded = EXP_DONE;
            return ret;
        }

        // check right child
        ret = explode(z->right, depth+1);
        // if done, quit. if need to go right, we can't service that here.
        if (ret.exploded == EXP_DONE || ret.exploded == EXP_R) return ret;
        if (ret.exploded == EXP_L) {
            // we can forward this over to the rightmost node in the left now!
            send_explosion(z->left, ret.val, EXP_R);
            ret.exploded = EXP_DONE;
            return ret;
        }

        // if we got here, neither child exploded.
    }

    return ret;
}

char split(struct num *z) {
    if (z->literal && z->val >= 10) {
        long long left = z->val / 2;
        long long right = z->val / 2 + z->val % 2;

        struct num *x, *y;
        x = malloc(sizeof(*x));
        y = malloc(sizeof(*y));
        x->literal = 1;
        x->val = left;
        y->literal = 1;
        y->val = right;
        z->literal = 0;
        z->left = x;
        z->right = y;

        return 1;
    } else if (!z->literal) {
        // check left, then right
        if (split(z->left)) return 1;
        if (split(z->right)) return 1;
    }

    return 0;
}

// In-place addition!
struct num *add(struct num *x, struct num *y) {
    struct num *z = malloc(sizeof(*z));
    z->literal = 0;
    z->left = x;
    z->right = y;
#ifdef DEBUG
    printf("adding: ");
    magnitude(z);
    printf("\n");
#endif
    
    // reduce
    while (1) {
        struct exp ex;
        ex = explode(z, 1);
        if (ex.exploded != EXP_NONE) {
#ifdef DEBUG
            printf("exploded... ");
            magnitude(z);
            putchar('\n');
#endif
            continue;
        }
        char spl = split(z);
        if (spl) {
#ifdef DEBUG
            printf("splitted... ");
            magnitude(z);
            putchar('\n');
#endif
            continue;
        }

        break;

    }

    return z;
}

void free_num(struct num *z) {
    if (z->literal) free(z);
    else {
        free_num(z->left);
        free_num(z->right);
        free(z);
    }
}


#define SIZE 1024
#define BUFCNT 128
int main(int argc, char **argv) {

    char buf[SIZE*BUFCNT];
    struct num *x = NULL;

    int nbuf = 0;
    while (fgets(buf + nbuf * SIZE, SIZE, stdin) != NULL) {
        const char *buf1 = buf + nbuf * SIZE;
        struct num *y = parse_num(&buf1);

#ifdef DEBUG
        printf("read: ");
        magnitude(y);
        printf("\n");
#endif
        if (x == NULL) x = y;
        else x = add(x, y);

        nbuf++;
        if (nbuf >= BUFCNT) {
            break;
        }
    }

    printf("part 1 magnitude = %lld\n", magnitude(x));

    free_num(x);

    long long magmax = 0;
    for (int i = 0; i < nbuf; i++) {
        for (int j = 0; j < nbuf; j++) {
            const char *buf1 = buf + i * SIZE;
            const char *buf2 = buf + j * SIZE;
            if (i == j) continue;

            struct num *x = parse_num(&buf1);
            struct num *y = parse_num(&buf2);
            struct num *z = add(x, y);

            long long mag = magnitude(z);
#ifdef DEBUG
            printf("magnitude of (%d,%d) = %lld\n", i, j, mag);
#endif
            if (mag > magmax) magmax = mag;
            free_num(z);
        }
    }

    printf("part 2 max magnitude = %lld\n", magmax);

}
