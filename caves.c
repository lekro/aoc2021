#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node {
    char name[7];
    char visited;
    struct node *neighbors[8];
    unsigned char nneighbors;
};

// depth-first search
int search(struct node *start, struct node *cur, struct node *end, int lim) {

    // did we find a path?
    if (cur == end) return 1;

    // ignore already visited nodes that aren't uppercase
    if (cur == start && start->visited) return 0;

    // allow visiting uppercase nodes as many times as we wish
    int upper = isupper(cur->name[0]);
    if (cur->visited >= lim && !upper) return 0;

    // we visited this node once
    cur->visited++;

    // if we visited this node already, then we are running out of the
    // "double-visit capacity", so pass a smaller limit. Otherwise, use the
    // limit provided by the caller
    int real_lim = (cur->visited > 1 && lim > 1 && !upper) ? lim - 1 : lim;

    // search neighbors
    int npaths = 0;
    for (int i = 0; i < cur->nneighbors; i++) {
        npaths += search(start, cur->neighbors[i], end, real_lim);
    }

    // backtracking
    cur->visited--;

    return npaths;
}

int main(int argc, char **argv) {

    struct node v[64], *start, *end;
    unsigned char nnodes;
    char names[32], *name1, *name2;

    while (fgets(names, 32, stdin) != NULL) {
        // parse names
        name1 = names;
        for (name2 = names; *name2 != '-'; name2++);
        *(name2++) = '\0';
        int i;
        for (i = name2 - names; names[i] != '\n' && names[i] != '\0' && i < 32; i++);
        names[i] = '\0';

        // find nodes by name
        struct node *v1 = NULL, *v2 = NULL;
        for (int j = 0; j < nnodes; j++) {
            if (strncmp(v[j].name, name1, sizeof(v[j].name)) == 0) v1 = &v[j];
            if (strncmp(v[j].name, name2, sizeof(v[j].name)) == 0) v2 = &v[j];
        }

        if (!v1) {
            v1 = &v[nnodes++];
            v1->visited = 0;
            v1->nneighbors = 0;
            strncpy(v1->name, name1, 7);
        }

        if (!v2) {
            v2 = &v[nnodes++];
            v2->visited = 0;
            v2->nneighbors = 0;
            strncpy(v2->name, name2, 7);
        }

        // add neighbors
        const size_t max_nneigh = sizeof(v1->neighbors) / sizeof(v1->neighbors[0]);
        if (v1->nneighbors == max_nneigh || v2->nneighbors >= max_nneigh) {
            fprintf(stderr, "too many neighbors!!!\n");
            exit(1);
        }
        v1->neighbors[v1->nneighbors++] = v2;
        v2->neighbors[v2->nneighbors++] = v1;

        // was this the start or end?
        if (strncmp(name1, "start", 5) == 0) start = v1;
        if (strncmp(name2, "start", 5) == 0) start = v2;
        if (strncmp(name1, "end", 5) == 0) end = v1;
        if (strncmp(name2, "end", 5) == 0) end = v2;
    }

    // ok let's run a search
    int npaths = search(start, start, end, 1);

    printf("part1 npaths = %d\n", npaths);

    // smaller caves can be visited twice
    npaths = search(start, start, end, 2);

    printf("part2 npaths = %d\n", npaths);
}
